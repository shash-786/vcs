#include "Add.h"
#include <algorithm>
#include <fstream>


void Command::Add::filter_work_tree_files(worktree& w, std::span<const std::string> args) 
{

}

void Command::Add::_read_work_tree_recursive(worktree& w, const std::string& base) {

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

void Command::Add::read_work_tree(worktree& w, const std::string& base) {
    // responsible for crawling down the directory from base
    // and gathering all the files in 'w'

    if (base[0] != '.') {
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
    _read_work_tree_recursive(w, base) ;
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
    std::string base = "." ;
    if (common_prefix != "") {
        base = common_prefix ;
    }

    read_work_tree(w, base) ;
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