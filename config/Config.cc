#include "Config.h"

int Configuration::LoadGlobalConfig()
{
    return 0;
}

const std::string &Configuration::get_git_name()
{
    return git_name ;
}

const std::string &Configuration::get_git_email()
{
    return git_email ;
}