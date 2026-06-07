#include <iostream>
#include "commands.hpp"

int main()
{
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      bukreev::executeCommand(cmd, std::cin, std::cout);
      std::cout << '\n';
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
