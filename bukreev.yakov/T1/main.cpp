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
    }
    catch(const std::logic_error& e)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
