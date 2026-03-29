#include <iostream>

#include <map>
#include <unordered_map>

void hi()
{
  std::cout << "aaaaaaaaaAAAAAAAAAAaaaaaaaa" << '\n';
}

int main()
{
  std::string cmd;

  while (std::cin >> cmd)
  {
//    std::cout << cmd << '\n';

    if (cmd == "hello")
    {
      hi();
    }
    else
    {
      std::cout << "not have this command" << '\n';
    }
    
  }

  if (!std::cin.eof())
  {
    std::cerr << "Error input\n";
    return 1;
  }
  
  


  int a;
  std::cin >> a;

  return 0;
}
