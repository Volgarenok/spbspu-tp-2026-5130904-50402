#include <fstream>
#include <iostream>

int main(int argc, char **argv)
{
  if (argc != 2) {
    std::cerr << "ERROR: expected a single file name argument\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "ERROR: cannot open the input file\n";
    return 2;
  }
}
