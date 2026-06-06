#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <stdexcept>

namespace pozdnyakov
{

  std::string readWord(std::istream &in)
  {
    std::string s;
    if (!(in >> s)) {
      in.clear();
      throw std::logic_error("read word error");
    }
    return s;
  }

  std::string readQuoted(std::istream &in)
  {
    std::string s;
    if (!(in >> std::quoted(s))) {
      in.clear();
      throw std::logic_error("read quoted error");
    }
    return s;
  }

  void cmdNote(std::istream &in, std::ostream &, Database &db)
  {
    auto name = readWord(in);
    if (db.count(name)) {
      throw std::logic_error("note already exists");
    }
    db[name] = std::make_shared< Note >(name);
  }

  void cmdLine(std::istream &in, std::ostream &, Database &db)
  {
    auto name = readWord(in);
    auto text = readQuoted(in);

    auto it = db.find(name);
    if (it == db.end()) {
      throw std::logic_error("note not found");
    }
    it->second->lines.push_back(text);
  }

  void cmdShow(std::istream &in, std::ostream &out, Database &db)
  {
    auto name = readWord(in);

    auto it = db.find(name);
    if (it == db.end()) {
      throw std::logic_error("note not found");
    }

    if (it->second->lines.empty()) {
      out << "\n";
    } else {
      for (const auto &line : it->second->lines) {
        out << line << "\n";
      }
    }
  }

  void cmdDrop(std::istream &in, std::ostream &, Database &db)
  {
    auto name = readWord(in);

    auto it = db.find(name);
    if (it == db.end()) {
      throw std::logic_error("note not found");
    }
    db.erase(it);
  }

  void cmdLink(std::istream &in, std::ostream &, Database &db)
  {
    auto from = readWord(in);
    auto to = readWord(in);

    auto it_from = db.find(from);
    auto it_to = db.find(to);
    if (it_from == db.end() || it_to == db.end()) {
      throw std::logic_error("note not found");
    }

    auto pf = it_from->second;
    auto pt = it_to->second;

    for (const auto &w : pf->links) {
      if (w.lock() == pt) {
        throw std::logic_error("already linked");
      }
    }
    pf->links.push_back(pt);
  }

  void cmdMind(std::istream &in, std::ostream &out, Database &db)
  {
    auto from = readWord(in);

    auto it_from = db.find(from);
    if (it_from == db.end()) {
      throw std::logic_error("note not found");
    }

    bool printed = false;
    for (const auto &w : it_from->second->links) {
      if (auto p = w.lock()) {
        out << p->name << "\n";
        printed = true;
      }
    }

    if (!printed) {
      out << "\n";
    }
  }

  void cmdHalt(std::istream &in, std::ostream &, Database &db)
  {
    auto from = readWord(in);
    auto to = readWord(in);

    auto it_from = db.find(from);
    auto it_to = db.find(to);
    if (it_from == db.end() || it_to == db.end()) {
      throw std::logic_error("note not found");
    }

    auto pf = it_from->second;
    auto pt = it_to->second;

    auto it = std::remove_if(pf->links.begin(), pf->links.end(), [&](const std::weak_ptr< Note > &w) {
      return w.lock() == pt;
    });

    if (it == pf->links.end()) {
      throw std::logic_error("link not found");
    }
    pf->links.erase(it, pf->links.end());
  }

  void cmdExpired(std::istream &in, std::ostream &out, Database &db)
  {
    auto from = readWord(in);

    auto it = db.find(from);
    if (it == db.end()) {
      throw std::logic_error("note not found");
    }

    int count = 0;
    for (const auto &w : it->second->links) {
      if (w.expired()) {
        count++;
      }
    }
    out << count << "\n";
  }

  void cmdRefresh(std::istream &in, std::ostream &, Database &db)
  {
    auto from = readWord(in);

    auto it = db.find(from);
    if (it == db.end()) {
      throw std::logic_error("note not found");
    }

    auto &lnks = it->second->links;
    lnks.erase(std::remove_if(lnks.begin(), lnks.end(),
      [](const std::weak_ptr< Note > &w) {
        return w.expired();
      }),
      lnks.end());
  }

}
