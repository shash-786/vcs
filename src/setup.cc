#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <system_error> 
#include <unistd.h>
#include <cstring>
#include <limits.h>
#include "common/result.h"
#include "common/errors.h"
#include "repo/Repository.h"

bool validate_head(const char* head) {
    std::ifstream file(head) ;
    if (!file.is_open()) {
        std::cerr << "Error opening HEAD file" << '\n';
        return 0;
    }

    std::string line ;
    if (std::getline(file, line)) {

        if (line.length() < 4) {
            std::cerr << "Not the correct format" << '\n';
            return 0;
        }

        if (line.compare(0, 4, "ref:") == 0) {
            int index = 4 ; 
            std::string against = "refs/" ;
            
            while (index < line.size()) {
                if (line[index] == ' ') {
                    index++ ;
                    continue ;
                } else if (line.compare(index, 5, against) != 0) {
                    return 0 ;
                } else {
                    return 1 ;
                }
            }
            // this is the case when there is nothing after refs: 
            return 0 ;
        }

        // TODO: If implemented CHECKOUT add support for detached head
        return 1 ;

    } else {
        std::cerr << "Cannot Read File --> .git/HEAD" << '\n';
        return 0;
    }
    return 0 ;
}

bool is_git_dir(const char* suspect) {
    char buffer[PATH_MAX] ;

    int len = strlen(suspect) ;
    if (len + 15 >= PATH_MAX) {
        return 0 ;
    }

    strcpy(buffer, suspect) ;

    if (access(buffer, X_OK)) {
        return 0 ;
    }

    strcpy(buffer + len, "/.git/objects") ;
    if (access(buffer, X_OK)) {
        return 0 ; 
    }

    strcpy(buffer + len, "/.git/refs") ;
    if (access(buffer, X_OK)) {
        return 0 ; 
    }

    strcpy(buffer + len, "/.git/HEAD") ;
    if (access(buffer, R_OK) || !validate_head(buffer)) {
        return 0 ; 
    }

    return 1 ;
}

Result<std::string, errorcode> setup_git_dir(Repo::Repository& _r) {
    char buffer[PATH_MAX] ;
    char* path = getcwd(buffer, PATH_MAX) ;

    if (!path) {
        std::cerr << "Error in retrieving current directory" << '\n';
        return errorcode::InternalError ;
    }

    int start = strlen(path) ;
    int len = strlen(path) ;

    if (len + 2 >= PATH_MAX) {
        std::cerr << "Path Too big" ;
        return errorcode::InternalError ;
    }

    // /home/shash/project/src
    //            s          ^len 
    while (!is_git_dir(".") && start > 0) {
        chdir("..") ;
        while (start > 0 && path[--start] != '/') ;
    }

    if (start == len) {
        return std::string() ;
    }

    if (start == 0) {
        std::cerr << "cannot run operation in root directory\n" ;
        return errorcode::NotAllowed ;
    }
    
    start++ ;
    path[len] = '/' ;
    path[++len] = '\0' ;

    
    if (strncmp(path + start, ".git/", 5) == 0) {
        std::cerr << "this operation must be run in a work tree\n" ;
        return errorcode::NotAllowed ;
    }

    std::string GIT_ROOT(buffer, start);
    _r.setRoot(std::filesystem::path(GIT_ROOT)) ;
    return std::string(buffer + start) ;
}

Result<std::string, errorcode> setup_git_dir(Repo::Repository& _r)
{
    std::error_code ec;
    std::filesystem::path rwd = std::filesystem::current_path(ec) ;
    std::filesystem::path cwd = rwd ;
    
    if (ec) {
        std::cerr << "Error retrieving path: " << ec.message() << '\n';
        return errorcode::InternalError ;
    }

    if (cwd.filename() == ".git") {
        std::cerr << "this operation must be run in a work tree\n" ;
        return errorcode::NotAllowed ;
    } else if (cwd.filename() == "/") {
        std::cerr << "cannot run operation in root directory\n" ;
        return errorcode::NotAllowed ;
    }

    bool found = false ;
    do {

        if (cwd.filename() == ".git") {
            std::cerr << "this operation must be run in a work tree\n" ;
            return errorcode::NotAllowed ;
        }

        if (is_git_dir(cwd.c_str())) {
            found = true ;
            break ;
        }

    } while ((cwd = cwd.parent_path()) != "/") ;

    if (!found) {
        std::cerr << "not a git repository (or any of the parent directories): .git\n" ;
        return errorcode::NotFound ;
    }

    std::string prefix = std::filesystem::relative(rwd, cwd).string();
    if (prefix.empty() || prefix == ".") {
        return std::string() ;
    }

    _r.setRoot(cwd) ;
    return prefix + '/' ;
}