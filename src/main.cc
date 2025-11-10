#include <cstdlib>
#include <iostream>
#include <boost/program_options.hpp>
#include "config/Config.h"
#include "commands/Add.h"
#include "commands/Init.h"
namespace po = boost::program_options;

extern Result<std::string, errorcode> setup_git_dir();

int main(int argc, char *argv[])
{
    int ret = 0;

    po::options_description menu("Allowed Commands");
    menu.add_options()
    ("help", "Display help information about Git")
    ("init", "Create an empty Git repository")
    ("add", "Add file contents to the indexx")
    ("commit", "Record changes to the repository");

    if (argc < 2)
    {
        std::cerr << "no arguments provided!\n";
        return EXIT_FAILURE;
    }

    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, argv, menu), vm);
        // Store the cmd args in vm
        po::notify(vm);
    }
    catch (const po::error &e)
    {

        std::cerr << "Error: " << e.what() << "\n";
        std::cerr << menu << "\n";
        return EXIT_FAILURE;
    }

    ret = Configuration::LoadGlobalConfig();
    if (!ret)
    {
        std::cerr << "Unable to load the global configuration file";
        return EXIT_FAILURE;
    }

    if (vm.count("help"))
    {
        std::cout << "Basic Content Tracker\n"  ;
        std::cout << menu << std::endl ;
    }

    std::string prefix = "" ;
    if (vm.count("init")) {
        // INIT COMMAND CALL 
    } else if (vm.count("add")) {
        
        auto res = setup_git_dir() ;
        if (res.err_present()) {
            return EXIT_FAILURE ;
        }
        std::string prefix = res.value() ;
        // ADD COMMAND CALL 

    } else if (vm.count("commit")) {
        // COMMIT COMMAND CALL 
    }

    return 0;
}
