#include <memory>
#include <string>
#include <vector>

namespace saldaev
{
  struct Note
  {
    std::string name;

    Note(std::string name);
    Note(const Note &) = delete;
    Note(Note &&) = delete;
    ~Note() = default;
    Note &operator=(const Note &) = delete;
    Note &operator=(Note &&) = delete;

    void addLine(std::string line);
    void addLink(std::shared_ptr< Note > note);
    void removeLink(const std::string &noteName);

    std::vector< std::weak_ptr< Note > > &getAllLinks();
    std::vector< std::shared_ptr< Note > > getActiveLinks();
    void show();

  private:
    std::vector< std::string > lines_;
    std::vector< std::weak_ptr< Note > > links_;
  };
}

int main()
{}

saldaev::Note::Note(std::string name):
  name(name)
{}
