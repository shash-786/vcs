#pragma once
#include <string>

class Configuration {
    /*
        A much simpler config loader
        we will just be parsing the name and
        email from the global configuration file
        present in the home directory 

        TODO: make user be able to set this
        git config --global user.name "Your Name"
        git config --global user.email "your.email@example.com"
    */
    public:
    static int LoadGlobalConfig() ;
    static const std::string& get_git_name() ;
    static const std::string& get_git_email() ;

    private:
    static std::string git_name ;
    static std::string git_email ;
};