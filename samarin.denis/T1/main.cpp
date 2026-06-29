#include <iostream>
#include "KnowledgeBase.hpp"
#include "commands.hpp"

int main()
{
  samarin::KnowledgeBase base;
  samarin::processCommands(base, std::cin, std::cout);
}
