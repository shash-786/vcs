#include "Add.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <string.h>
#include <fnmatch.h>
#include <sys/stat.h>

bool Command::Add::match (const std::string& to_match, const std::string& with, std::vector<bool>& seen, int i) {
    // Case where arg is '.' so arg will be ''
    if (!to_match.size())
        return seen[i] = true ;
    
    if (with.compare(0, to_match.size(), to_match)) {
        // meaning `with` is not matching upto arg
        // case could be `arg` = 'README' with = 'EMDAER'
        // or `arg` = 'src/main.cc' `with` = 'src/main.py'
        // or `arg` = '*.cc' with = 'main.cc'
        if (fnmatch(to_match.c_str(), with.c_str(), 0) == 0) {
            return seen[i] = true ;
        } else {
            return false ;
        }
    }

    /*
    This would cover both the cases where the 
    2 suspects are exactly equal and the one case
    where arg = 'src' and the with = 'src/main.cc'

    So recursive and equal are being matched here
    
    but wait this would also return true
    for `arg` = 'src' and with = 'src_backup'
    */
    if (to_match.size() == with.size())
        return seen[i] = true ;
    // if `arg` = 'src/' or if `arg` = 'src and with = 'src/main.cc'
    if (to_match.back() == '/' || with[to_match.size()] == '/') 
        return seen[i] = true ;
    return false ;
}

void Command::Add::filter_work_tree_files(worktree& w, std::span<const std::string> args) 
{
    int n = args.size() ;
    std::vector<bool> seen(n, 0) ;

    int dst = 0 ;

    for (int src = 0; src < w.files.size(); src++) {
        for (int i = 0; i < args.size(); i++) {
            if (match(args[i], w.files[src].name, seen, i)) {
                std::cout << "matched " << args[i] << " with " << w.files[src].name << std::endl ;
                w.files[dst++] = std::move(w.files[src]) ;
            }
        }
    }
    w.files.resize(dst) ;

    for (int i = 0; i < args.size(); i++) {
        if (!seen[i]) {
            fprintf(stderr, "%s did not match any pathspec", args[i].c_str()) ;
            exit(1) ;
        }
    }
}

bool Command::Add::excluded (const std::string& suspect, worktree& w) {
    /*
    we are checking for every dir entry whether it matches any of the ignore rules
    we use primarily the function fnmatch for matching the patterns
    */
    // TODO: make this function match the rule 'src/' with src
    // currently it doesn't do that
    for (int i = w.dr->nr - 1; i >= 0; i--) {
        const directory_ignore_ruleset::entry& entry = w.dr->rules[i] ;
        const std::string& pattern = entry.rule ;
        const std::string& base = entry.base ;
        int it = 0 ;
        bool to_exclude = true ;

        // If a rule starts with '!'
        // That means it is **TO BE INCLUDED**
        if (pattern[0] == '!') {
            to_exclude = false ;
            ++it ;
        }

        auto pos = pattern.find('/', it) ;
        if (pos == std::string::npos) {
            // The exclude rule does not contain any '/'
            // this means it is a simple rule either regex based or name based
            pos = suspect.rfind('/') ;
            const char* ptr = pos == std::string::npos ? suspect.c_str() : suspect.c_str() + pos + 1 ;
            if (fnmatch(pattern.c_str() + it, ptr, 0) == 0) {
                return to_exclude ;
            }

        } else {

            if (pattern[it] == '/') {
                ++it ;
            }
            // case where suspect = 'a'
            // and we are reading rules of src/ directory so base will be 'src/' rule could be '/api'
            if (suspect.size() < base.size())
                continue ;

            // case where suspect = 'api/main.cc'
            // and our rule is base = 'src/' rule = 'main.cc'
            // this will check whether position 3 in suspect is '/'
            if (base.size() && suspect[base.size() - 1] != '/')
                continue ;
            
            // this will match the suspect = 'api/' and base = 'src/'
            if (suspect.compare(0, base.size(), base) != 0) 
                continue ;
            
            // finally do a match of the pattern
            // let's say pattern is '*.cc' and base is 'src/'
            // suspect is 'src/main.cc' we will compare
            // fnmatch('*.cc' , "main.cc", FNM_PATHNAME)
            // FNM_PATHNAME --> this flag is short for not greedy
            // without it '*.txt' will match both 'f.txt' and 'a/h.txt'
            // "*.cpp"	"src/main.cpp"	FNM_NOMATCH 
            if (fnmatch(pattern.c_str() + it, suspect.c_str() + base.size(), FNM_PATHNAME) == 0) {
                return to_exclude ;
            }
        }
    }
    return false ;
}

void Command::Add::dir_add_entry (worktree& w, std::string& name, int len) {
    worktree_entry new_entry ;
    new_entry.name = name ;
    w.files.push_back(std::move(new_entry)) ;
}

int Command::Add::_read_work_tree_recursive(worktree& w, const std::string& path, const std::string& base) {
    DIR* dir = opendir(path.c_str()) ;
    std::string fullname = base ;
    size_t baselen = fullname.size() ;

    int contents = 0 ;
    if (dir) {
        struct dirent* d ;
        int excludes = push_excludes_from(w, base) ;
        while ((d = readdir(dir)) != nullptr) {

            auto type = d->d_type ;
            // Sometimes the d_type field can be DT_UNKNOWN, especially on certain filesystems.
            // Check using the lstat sys call to get the conecrete type
            if (type == DT_UNKNOWN) {
                struct stat st ;
                if (lstat(fullname.c_str(), &st) == 0) {
                    if (S_ISDIR(st.st_mode)) type = DT_DIR ;
                    else if (S_ISREG(st.st_mode)) type = DT_REG ;
                    else if (S_ISLNK(st.st_mode)) type = DT_LNK ;
                }
            }
            
            // Check whether the current entry is "." or ".." or ".git" if yes then skip it
            if (d->d_name[0] == '.' && (d->d_name[1] == '\0' || (d->d_name[1] == '.' && d->d_name[2] == '\0') || strcmp(d->d_name, ".git") == 0) ) {
                continue ;
            }

            // After every iteration we resize to the base length size
            // this is because we are reusing the same buffer
            // So for example if base is "src/"
            // first entry would be main.cc so fullname = "src/main.cc"
            // next entry is utils/ so we need to rewind back to "src/" and then
            // append "utils/" to it to make "src/utils/"
            fullname.resize(baselen) ;
            fullname.append(d->d_name) ;
            int de_name_len = strlen(d->d_name) ;

            if (excluded(fullname, w))
                continue ; 

            switch (type)
            {
            // FIXED: Added curly braces to create a specific scope for 'new_base'.
            // Without braces, 'new_base' is visible to the entire switch block.
            // If execution jumped to case DT_REG, it would bypass the constructor,
            // leaving 'new_base' uninitialized. However, the compiler knows the 
            // destructor must run at the end of the switch. Destroying an 
            // uninitialized object causes Undefined Behaviour/crashes, so the
            // compiler forbids it Although it is now being constructed else where 
            // remember the error
            case DT_DIR:
                fullname.push_back('/') ;
                contents += _read_work_tree_recursive(w, fullname, fullname) ;
                continue ; 
            case DT_REG:
                contents++ ;
                break ; 
            case DT_LNK:
                contents++ ;
                break ; 
            default:
                fprintf(stderr, "only regular files allowed skipping %s", d->d_name) ;
                continue ;
            }
            dir_add_entry(w, fullname, de_name_len) ;
        }
        // For every directory we read we need to pop the excludes we pushed
        // This is to ensure if we are inside the folder src/ we need to pop
        // rules of src/ because we don't want them to affect the parent directory or any
        // other directory in parent
        pop_excludes(w, excludes) ;
    }
    closedir(dir) ;
    return contents ;
}

void Command::Add::pop_excludes(worktree& w, int to_pop) {
    std::vector<directory_ignore_ruleset::entry>& rules = w.dr->rules ;
    while (to_pop > 0) {
        if (rules.empty()) {
            fprintf(stderr, "popping more than what was pushed") ;
            exit(1) ;
        }
        w.dr->rules.pop_back() ;
        // missed decrementing the total rules
        w.dr->nr-- ;
        to_pop-- ;
    }
}

int Command::Add::push_excludes_from(worktree& w, const std::filesystem::path& wd) {
    std::filesystem::path from = wd / ".gitignore" ;
    std::ifstream file(from) ;

    if (!file.is_open()) {
        return 0 ;
    }

    std::string line = "" ;
    int count = 0 ;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue ; 
        }
        ++count ;
        directory_ignore_ruleset::entry e ;
        
        // No std::move on wd.string(): it's a temporary (rvalue) and is already moved automatically.  
        e.base = wd.string() ;
        // Use std::move on line: it's a variable (lvalue) we can steal from to avoid a copy.
        e.rule = std::move(line) ;
        w.dr->rules.push_back(std::move(e)) ;
    }
    
    file.close() ;
    w.dr->nr += count ;
    return count ;
}

void Command::Add::read_work_tree(worktree& w, const std::string& path, const std::string& base) {
    // responsible for crawling down the directory from base
    // and gathering all the files in 'w'

    if (base != "") {
        /* 
        common prefix exist **OPTIMISATION** 
        Thanks Linus :)
        only crawl from common prefix and not from

        if common prefix exists you are NOT starting from root
        So push all the exclude rules leading upto the prefix

        if prefix = "src/api/"

        inlcude .gitignore rules from "", "src/" "src/api/"
        */
        std::filesystem::path cwd = "" ;
        std::filesystem::path iter = base ;
        
        // Complexity O (pos + 1)
        // int pos = -1 ;
        // do {
        //     if (pos != -1) {
        //         cwd = cwd / base.substr(0, pos + 1) ;
        //     }

        //     push_excludes_from(cwd) ;
        //     pos = base.find('/', pos + 1) ;
        // } while (pos != std::string::npos) ;
        
        push_excludes_from(w, cwd) ;
        for (std::filesystem::path component: iter) {
            if (component.empty()) {
                continue;
            }
            cwd = cwd / component ;
            push_excludes_from(w, cwd) ;
        }
    }
    _read_work_tree_recursive(w, path, base) ;
    std::sort(w.files.begin(), w.files.end(), [](const worktree_entry& a, const worktree_entry& b) {
        return a.name < b.name ;
    }) ;
}

std::string Command::Add::find_common_root(std::span<const std::string> args) 
{
    std::string first = args[0] ;
    int pos = 0 ;
    if ((pos = first.rfind('/')) == std::string::npos) {
        return "" ;
    }

    for (int i = 1; i < args.size() ; i++) {

        // if pos is 6 that means the current prefix length is 7
        // if the size of the string you are comparing is less that 7
        // no point in calling the compare function
        while (args[i].size() < pos + 1 || first.compare(0, pos + 1, args[i], 0, pos + 1) != 0) {

            /* 
            ["src/api/main.cc", "src/api/main1.cc", "src/main.cc", "main.cc"]

            at i = 2 pos will be shifted to 3 so prefix would be src/
            at i = 3 pos will be shitted to std::string::npos 

            so OVERALL common prefix will be ""
            */
            pos = first.rfind('/', pos - 1) ;
            if (pos == std::string::npos) 
                return "" ;
        }
    }
    return first.substr(0, pos + 1) ;
}

void Command::Add::get_and_filter_work_tree_files(worktree& w, const std::vector<std::string>& args)
{
    std::string common_prefix = find_common_root(args) ;
    std::string base = "" ;
    std::string path = "." ;
    if (common_prefix != "") {
        path = base = common_prefix ;
    }

    read_work_tree(w, path, base) ;
    filter_work_tree_files(w, args) ;
}

std::string Command::Add::_resolve(std::string& path) {
    std::string new_path = "" ;
    int prefixLen = prefix.size() ;
    int len = path.size() , i = 0 ;

    while (i < len) {
        // to cover both case ./ and ../
        if (path[i] != '.')
            break ;

        i++ ;

        // '.' only 
        if (i == len) {
            break ;
        }

        // './' case
        if (path[i] == '/') {
            i++ ;
            continue ;
        }

        // maybe .gitignore or .env?
        if (path[i] != '.') {
            break ;
        }
        
        // '../' or '..' case
        i++ ;
        
        // maybe ..s or ...t/
        if (i != len && path[i] != '/') {
            break ;
        } else if (i == len) {
            // '..' case 
        } else {
            // '../' case 
            i++ ;
        }

        do {
            /*
                prefix = src/app/ 
                prefixLen = 8 

                first .. 
                we will  reach position 4
            */
            if (!prefixLen) {
                fprintf(stderr, "%s is not inside the repository", path.c_str()) ;
                exit(1) ;
            }
            --prefixLen ;
        } while (prefixLen && prefix[prefixLen - 1] != '/') ;
    }

    new_path.append(prefix, prefixLen) ;
    new_path.append(path, len - i) ;
    return new_path ;
}

void Command::Add::resolve_paths(std::vector<std::string>& paths)
{
    for (int i = 0; i < paths.size(); i++) {
        paths[i] = _resolve(paths[i]) ;
    }
}

int Command::Add::execute(std::vector<std::string> &args) {
    // TODO: Implement logic for flags
    // -n to only show the the files which will be added
    // -f to show ignored files
    resolve_paths(args) ;
    auto w = std::make_unique<worktree>() ;
    w->dr = std::make_unique<directory_ignore_ruleset>() ;
    get_and_filter_work_tree_files(*w, args) ;

    // TODO: Index file Logic
    return 0 ;
}