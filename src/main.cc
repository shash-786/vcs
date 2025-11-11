#include <cstdlib>
#include <iostream>
#include <boost/program_options.hpp>
#include "config/Config.h"
#include "fs/FSManager.h"
#include "repo/Repository.h"
#include "commands/Add.h"
#include "commands/Init.h"
namespace po = boost::program_options;

extern Result<std::string, errorcode> setup_git_dir(Repo::Repository& _r);

int main(int argc, char *argv[])
{
    int ret = 0;

    po::options_description menu("Allowed Commands");
    menu.add_options()
    ("help", "Display help information about Git")
    ("init", "Create an empty Git repository")
    ("add", "Add file contents to the indexx")
    ("rm", "Remove files from the working tree and from the index");
    ("commit", "Record changes to the repository");
    ("log", " Show commit logs");

    if (argc < 2)
    {
        std::cerr << "no arguments provided!\n";
        return EXIT_FAILURE;
    }

    po::options_description generic_ops("Allowed options");
    generic_ops.add_options()
        ("help", "Display help information");

    po::options_description hidden_ops("Hidden options");
    hidden_ops.add_options()
        ("command", po::value<std::string>(), "command")
        ("subargs", po::value<std::vector<std::string>>(), "arguments for command") ;

    po::options_description all_ops;
    all_ops.add(generic_ops).add(hidden_ops);
    po::positional_options_description pos_desc;
    pos_desc.add("command", 1); // The first positional arg is "command"
    pos_desc.add("subargs", -1); // All the rest are "subargs"

    po::variables_map vm;
    try
    {
        po::store(po::command_line_parser(argc, argv)
                      .options(all_ops)
                      .positional(pos_desc)
                      .run(),
                  vm);

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

    if (!vm.count("command")) {
        std::cerr << "No command specified.\n";
        std::cout << menu << std::endl;
        return EXIT_FAILURE;
    }

    std::string command = vm["command"].as<std::string>();

    std::vector<std::string> sub_args;
    if (vm.count("subargs")) {
        sub_args = vm["subargs"].as<std::vector<std::string>>();
    }

    auto _r = Repo::Repository::getInstance() ;
    FS::FSManager _f ;

    std::string prefix = "" ;
    std::unique_ptr<Command::Command> c ;

    if (command == "init") {
        c = std::make_unique<Command::Init>(_f, _r) ;

    } else if (command == "add") {
        
        auto setup = setup_git_dir(_r) ;
        if (setup.err_present()) 
            return EXIT_FAILURE ;
        c = std::make_unique<Command::Add>(_f, _r, setup.value()) ;

    } else if (command == "rm") {

        auto setup = setup_git_dir(_r) ;
        if (setup.err_present()) {
            return EXIT_FAILURE ;
        }
        // RM COMMAND CALL 
    } else if (command == "commit") {
        // COMMIT COMMAND CALL 
    } else if (command == "log") {
        // LOG COMMAND CALL 
    }

    if (c) {
        return c->execute(sub_args) ;
    }
    return 1;
}
