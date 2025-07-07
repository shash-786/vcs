#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "no arguments provided!\n";
    return EXIT_FAILURE;
  }

  std::string command = argv[1];

  return 0;
}
