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
	std::vector< std::weak_ptr< Note > links;
}

void noteCommand(std::istream& in, std::ostream&, d_t& data)
{
	std::string name;
	in >> name;
	if (data.find(name) == data.cend())
	{
		data[name] = std::make_shared< Note >(name);
	}
	else
	{
		throw std::logic_error("Note already exist.");
	}
}

void lineCommand(std::istream& in, std::ostream&, d_t& data)
{
	std::string name, str;
	in >> name;
	auto it = data.find(name);
	if (it != data.cend())
	{
		in >> std::quoted(str);
		it->second->text.push_back(str);
	}
	else
	{
		throw std::logic_error("Note with this name doesn't exist.");
	}
}

void showCommand(std::istream& in, std::ostream& out, d_t& data)
{
	std::string name;
	in >> name;
	auto it = data.find(name);
	if (it != data.cend())
	{
		auto yait = it->second->text.cbegin();
		for (; yait != it->second->text.cend(); ++yait)
		{
			out << *yait << '\n';
		}
	}
	else
	{
		throw std::logic_error("Note with this name doesn't exist.");
	}
}

void dropCommand(std::istream& in, std::ostream&, d_t& data)
{
	std::string name;
	in >> name;
	auto it = data.find(name);
	if (it != data.cend())
	{
		data.erase(it);
	}
	else
	{
		throw std::logic_error("Note with this name doesn't exist.");
	}
}

int main()
{
	using d_t = std::unordered_map< std::string, std::shared_ptr< Note > >;
	d_t data;
	using cmd_t = void(*)(std::istream&, std::ostream&, d_t&);
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
	while (std::cin >> cmd)
	{
		try
		{
			cmds.at(cmd)(std::cin, std::cout, data);
		}
		catch (const std::out_of_range&)
		{
			std::cout << "<INVALID COMMAND>\n";
			auto toignore = std::numeric_limits< std::streamsize >::max();
			std::cin.ignore(toignore, '\n');
		}
		catch (const std::logic_error& e)
		{
			std::cout << "<INVALID COMMAND: >" << e.what() << ">\n";
		}
	}
	if (!std::cin.eof())
	{
		std::cerr << "Bad input\n";
		return 1;
	}
}
