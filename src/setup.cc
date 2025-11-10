#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <system_error> 
#include <unistd.h>
#include "common/result.h"
#include "common/errors.h"

bool validate_head(const std::filesystem::path& head) {
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
                    break ;
                }
            }
        }

        // TODO: If implemented CHECKOUT add support for detached head

    } else {
        std::cerr << "" << '\n';
        return 0;
    }
}

bool is_git_dir(const std::filesystem::path& p) {
    std::filesystem::path suspect = p / ".git" ;
    if (access(suspect.c_str(), X_OK)) {
        return 0 ;
    }

    suspect = p / ".git" / "objects" ;
    if (access(suspect.c_str(), X_OK)) {
        return 0 ; 
    }

    suspect = p / ".git" / "refs" ;
    if (access(suspect.c_str(), X_OK)) {
        return 0 ; 
    }

    suspect = p / ".git" / "HEAD" ;
    if (access(suspect.c_str(), R_OK) || !validate_head(suspect)) {
        return 0 ; 
    }

    return 1 ;
}

Result<std::string, errorcode> setup_git_dir()
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

        if (is_git_dir(cwd)) {
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
        return errorcode::NotFound ;
    }
    return prefix + '/' ;
}