#include "Add.h"
#include <algorithm>

int Command::Add::execute(std::vector<std::string> &args) {
    // TODO: Implement logic for flags
    // -n to only show the the files which will be added
    // -f to show ignored files
    resolve_paths(args) ;
    return 0 ;
}

void Command::Add::resolve_paths(std::vector<std::string>& paths)
{
    for (int i = 0; i < paths.size(); i++) {
        paths[i] = _resolve(paths[i]) ;
    }
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