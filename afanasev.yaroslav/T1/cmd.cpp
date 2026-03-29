#include <iostream>
#include <unordered_map>
#include "cmd.hpp"
#include "note.hpp"

#include <iosfwd>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <iomanip>

void afanasev::noteCmd(std::istream & in, std::ostream &, note_t & db)
{
	std::string name;
	in >> name;

	if (db.find(name) == db.end())
	{
		db[name] = std::make_shared< Note >();
	}
	else
	{
		throw std::logic_error("This note have");
	}
}

void afanasev::lineCmd(std::istream & in, std::ostream & out, note_t & db)
{
	std::string name, text;
	in >> name;

	in >> std::quoted(text);

  try
	{
    db.at(name)->text.push_back(text);
  }
	catch (const std::out_of_range &)
	{
    throw std::logic_error("not have note with this name");
  }
}

void afanasev::showCmd(std::istream & in, std::ostream & out, note_t & db)
{
	std::string name;
	in >> name;

	try
	{
		for (const std::string & line : db.at(name)->text)
		{
			out << line << '\n';
		}
	}
	catch (const std::out_of_range &)
	{
		throw std::logic_error("not have note with this name");
	}
}

void afanasev::dropCmd(std::istream & in, std::ostream & out, note_t & db){}


void afanasev::linkCmd(std::istream & in, std::ostream & out, note_t & db){}

void afanasev::haltCmd(std::istream & in, std::ostream & out, note_t & db){}

void afanasev::mindCmd(std::istream & in, std::ostream & out, note_t & db){}

void afanasev::expiredCmd(std::istream & in, std::ostream & out, note_t & db){}

void afanasev::refreshCmd(std::istream & in, std::ostream & out, note_t & db){}
