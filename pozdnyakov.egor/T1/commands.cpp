#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <vector>

namespace
{
  bool isExpired(const std::weak_ptr< pozdnyakov::Note > &w)
  {
    return w.expired();
  }

  bool dfsFindCycle(const std::shared_ptr< pozdnyakov::Note > &current,
                    const std::shared_ptr< pozdnyakov::Note > &target, int edgesLeft,
                    std::vector< std::shared_ptr< pozdnyakov::Note > > &path, std::ostream &out)
  {
    if (edgesLeft <= 0) {
      return false;
    }

    for (const std::weak_ptr< pozdnyakov::Note > &w : current->links) {
      std::shared_ptr< pozdnyakov::Note > neighbor = w.lock();
      if (!neighbor) {
        continue;
      }

      if (neighbor == target) {
        out << path[0]->name << " " << (path.size() > 1 ? path[1]->name : target->name);
        for (size_t i = 1; i < path.size(); ++i) {
          std::string nextName = (i + 1 < path.size()) ? path[i + 1]->name : target->name;
          out << "\n" << path[i]->name << " " << nextName;
        }
        return true;
      }

      bool inPath = false;
      for (size_t i = 0; i < path.size(); ++i) {
        if (path[i] == neighbor) {
          inPath = true;
          break;
        }
      }

      if (inPath) {
        continue;
      }

      if (edgesLeft > 1) {
        path.push_back(neighbor);
        if (dfsFindCycle(neighbor, target, edgesLeft - 1, path, out)) {
          return true;
        }
        path.pop_back();
      }
    }
    return false;
  }
}

void pozdnyakov::cmdNote(std::istream &in, std::ostream &, pozdnyakov::Database &db)
{
  std::string name;
  in >> name;
  if (in.fail()) {
    throw std::logic_error("invalid input");
  }

  std::pair< pozdnyakov::Database::iterator, bool > res = db.emplace(name, std::make_shared< pozdnyakov::Note >(name));
  if (!res.second) {
    throw std::logic_error("note already exists");
  }
}

void pozdnyakov::cmdLine(std::istream &in, std::ostream &, pozdnyakov::Database &db)
{
  std::string name;
  std::string text;
  in >> name >> std::quoted(text);
  if (in.fail()) {
    throw std::logic_error("invalid input");
  }

  db.at(name)->lines.push_back(text);
}

void pozdnyakov::cmdShow(std::istream &in, std::ostream &out, pozdnyakov::Database &db)
{
  std::string name;
  in >> name;
  if (in.fail()) {
    throw std::logic_error("invalid input");
  }

  const std::vector< std::string > &lines = db.at(name)->lines;

  if (!lines.empty()) {
    out << lines.front();
    for (size_t i = 1; i < lines.size(); ++i) {
      out << "\n" << lines[i];
    }
  }
}

void pozdnyakov::cmdDrop(std::istream &in, std::ostream &, pozdnyakov::Database &db)
{
  std::string name;
  in >> name;
  if (in.fail()) {
    throw std::logic_error("invalid input");
  }

  db.at(name);
  db.erase(name);
}

void pozdnyakov::cmdLink(std::istream &in, std::ostream &, pozdnyakov::Database &db)
{
  std::string from;
  std::string to;
  in >> from >> to;
  if (in.fail()) {
    throw std::logic_error("invalid input");
  }

  std::shared_ptr< pozdnyakov::Note > pf = db.at(from);
  std::shared_ptr< pozdnyakov::Note > pt = db.at(to);

  for (const std::weak_ptr< pozdnyakov::Note > &w : pf->links) {
    if (w.lock() == pt) {
      throw std::logic_error("already linked");
    }
  }
  pf->links.push_back(pt);
}

void pozdnyakov::cmdMind(std::istream &in, std::ostream &out, pozdnyakov::Database &db)
{
  std::string from;
  in >> from;
  if (in.fail()) {
    throw std::logic_error("invalid input");
  }

  std::shared_ptr< pozdnyakov::Note > pf = db.at(from);

  auto it = pf->links.begin();
  while (it != pf->links.end() && it->expired()) {
    ++it;
  }

  if (it != pf->links.end()) {
    std::shared_ptr< pozdnyakov::Note > p = it->lock();
    out << p->name;
    ++it;

    for (; it != pf->links.end(); ++it) {
      std::shared_ptr< pozdnyakov::Note > nextP = it->lock();
      if (nextP) {
        out << "\n" << nextP->name;
      }
    }
  }
}

void pozdnyakov::cmdHalt(std::istream &in, std::ostream &, pozdnyakov::Database &db)
{
  std::string from;
  std::string to;
  in >> from >> to;
  if (in.fail()) {
    throw std::logic_error("invalid input");
  }

  std::shared_ptr< pozdnyakov::Note > pf = db.at(from);
  std::shared_ptr< pozdnyakov::Note > pt = db.at(to);

  bool found = false;
  for (auto it = pf->links.begin(); it != pf->links.end(); ++it) {
    if (it->lock() == pt) {
      pf->links.erase(it);
      found = true;
      break;
    }
  }

  if (!found) {
    throw std::logic_error("link not found");
  }
}

void pozdnyakov::cmdExpired(std::istream &in, std::ostream &out, pozdnyakov::Database &db)
{
  std::string from;
  in >> from;
  if (in.fail()) {
    throw std::logic_error("invalid input");
  }

  std::shared_ptr< pozdnyakov::Note > pf = db.at(from);

  int count = 0;
  for (const std::weak_ptr< pozdnyakov::Note > &w : pf->links) {
    if (w.expired()) {
      count++;
    }
  }
  out << count;
}

void pozdnyakov::cmdRefresh(std::istream &in, std::ostream &, pozdnyakov::Database &db)
{
  std::string from;
  in >> from;
  if (in.fail()) {
    throw std::logic_error("invalid input");
  }

  std::shared_ptr< pozdnyakov::Note > pf = db.at(from);

  pf->links.erase(std::remove_if(pf->links.begin(), pf->links.end(), isExpired), pf->links.end());
}

void pozdnyakov::cmdLoop(std::istream &in, std::ostream &out, pozdnyakov::Database &db)
{
  std::string startName;
  int maxNodes;
  in >> startName >> maxNodes;
  if (in.fail() || maxNodes < 0) {
    throw std::logic_error("invalid input");
  }

  std::shared_ptr< pozdnyakov::Note > startNode = db.at(startName);
  std::vector< std::shared_ptr< pozdnyakov::Note > > path;
  path.push_back(startNode);

  if (!dfsFindCycle(startNode, startNode, maxNodes + 1, path, out)) {
    throw std::logic_error("no cycle");
  }
}
