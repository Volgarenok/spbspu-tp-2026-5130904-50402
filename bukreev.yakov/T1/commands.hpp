#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>

namespace bukreev
{
  void executeCommand(std::string cmd, std::istream& in, std::ostream& out);

  void noteCommand(std::istream& in);
  void lineCommand(std::istream& in);
  void showCommand(std::istream& in, std::ostream& out);
  void dropCommand(std::istream& in);
  void linkCommand(std::istream& in);
  void haltCommand(std::istream& in);
  void mindCommand(std::istream& in, std::ostream& out);
  void expiredCommand(std::istream& in, std::ostream& out);
  void refreshCommand(std::istream& in);
}

#endif
