#include "op.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>

void novikov::note(std::istream& in, std::ostream&, mapNotes& db)
{
    std::string name;
    in >> name;
    if(db.find(name) != db.end())
    {
        throw std::logic_error("Note already exists");
    }
    auto new_note = std::make_shared< Note >(note);
    db[name] = new_note;
}