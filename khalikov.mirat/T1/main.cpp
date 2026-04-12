#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include <memory>

struct Note
{
  std::string name;
  std::vector< std::string > text;
  std::vector< std::weak_ptr< Note > > links;

  Note(const std::string &nname):
    name(nname)
  {}
};

using d_t = std::unordered_map< std::string, std::shared_ptr< Note > >;

void noteCommand(std::istream &in, std::ostream &, d_t &data)
{
  std::string name;
  in >> name;
  if (data.find(name) == data.cend()) {
    data[name] = std::make_shared< Note >(name);
  } else {
    throw std::logic_error("Note already exist.");
  }
}

void lineCommand(std::istream &in, std::ostream &, d_t &data)
{
  std::string name, str;
  in >> name;
  auto it = data.find(name);
  if (it != data.cend()) {
    in >> std::quoted(str);
    it->second->text.push_back(str);
  } else {
    throw std::logic_error("Note with this name doesn't exist.");
  }
}

void showCommand(std::istream &in, std::ostream &out, d_t &data)
{
  std::string name;
  in >> name;
  auto it = data.find(name);
  if (it != data.cend()) {
    auto yait = it->second->text.cbegin();
    for (; yait != it->second->text.cend(); ++yait) {
      out << *yait << '\n';
    }
  } else {
    throw std::logic_error("Note with this name doesn't exist.");
  }
}

void dropCommand(std::istream &in, std::ostream &, d_t &data)
{
  std::string name;
  in >> name;
  auto it = data.find(name);
  if (it != data.cend()) {
    data.erase(it);
  } else {
    throw std::logic_error("Note with this name doesn't exist.");
  }
}

void linkCommand(std::istream &in, std::ostream &, d_t &data)
{
  std::string name, yaname;
  in >> name >> yaname;
  auto it = data.find(name);
  auto yait = data.find(yaname);
  if (it != data.cend() && yait != data.cend()) {
    auto itlink = it->second->links.cbegin();
    for (; itlink != it->second->links.cend(); ++itlink) {
      if (itlink->lock() && itlink->lock()->name == yaname) {
        throw std::logic_error("This note already linked with this");
      }
    }
    it->second->links.push_back(yait->second);
  } else {
    throw std::logic_error("Note with this name doesn't exist.");
  }
}

void haltCommand(std::istream &in, std::ostream &, d_t &data)
{
  std::string name, yaname;
  in >> name >> yaname;
  auto it = data.find(name);
  auto yait = data.find(yaname);
  if (it != data.cend() && yait != data.cend()) {
    bool deleted = false;
    auto itlink = it->second->links.begin();
    for (; itlink != it->second->links.end(); ++itlink) {
      if (itlink->lock() && itlink->lock()->name == yaname) {
        it->second->links.erase(itlink);
        deleted = true;
        break;
      }
    }
    if (!deleted) {
      throw std::logic_error("Note is not linked with this.");
    }
  } else {
    throw std::logic_error("Note with this name doesn't exist.");
  }
}

void mindCommand(std::istream &in, std::ostream &out, d_t &data)
{
  std::string name;
  in >> name;
  auto it = data.find(name);
  if (it != data.cend()) {
    auto itlink = it->second->links.begin();
    for (; itlink != it->second->links.end(); ++itlink) {
      if (itlink->lock()) {
        out << itlink->lock()->name << '\n';
      }
    }
  } else {
    throw std::logic_error("Note with this name doesn't exist.");
  }
}

void expiredCommand(std::istream &in, std::ostream &out, d_t &data)
{
  std::string name;
  in >> name;
  auto it = data.find(name);
  if (it != data.cend()) {
    size_t count = 0;
    auto itlink = it->second->links.begin();
    for (; itlink != it->second->links.end(); ++itlink) {
      if (itlink->expired()) {
        count++;
      }
    }
    out << count << '\n';
  } else {
    throw std::logic_error("Note with this name doesn't exist.");
  }
}

void refreshCommand(std::istream &in, std::ostream &, d_t &data)
{
  std::string name;
  in >> name;
  auto it = data.find(name);
  if (it != data.cend()) {
    auto itlink = it->second->links.begin();
    for (; itlink != it->second->links.end();) {
      if (itlink->expired()) {
        itlink = it->second->links.erase(itlink);
      } else {
        ++itlink;
      }
    }
  } else {
    throw std::logic_error("Note with this name doesn't exist.");
  }
}

int main()
{
  d_t data;
  using cmd_t = void (*)(std::istream &, std::ostream &, d_t &);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = noteCommand;
  cmds["line"] = lineCommand;
  cmds["show"] = showCommand;
  cmds["drop"] = dropCommand;
  cmds["link"] = linkCommand;
  cmds["halt"] = haltCommand;
  cmds["mind"] = mindCommand;
  cmds["expired"] = expiredCommand;
  cmds["refresh"] = refreshCommand;
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, data);
    } catch (const std::out_of_range &) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error &e) {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }
}
