#include <memory>
#include <string>
#include <vector>

namespace saldaev
{
  struct Note
  {
    std::string name;

    Note(std::string name);
    ~Note();

    void addLine(std::string line);
    void addLink(std::shared_ptr< Note > note);
    void removeLink(const std::string &noteName);

    std::vector< std::weak_ptr< Note > > &getAllLinks();
    std::vector< std::shared_ptr< Note > > getActiveLinks();
    void show();

  private:
    std::vector< std::string > lines;
    std::vector< std::weak_ptr< Note > > links;
  };

}
int main()
{}
