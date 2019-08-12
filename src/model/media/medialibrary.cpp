#include "medialibrary.h"

#include "freespace.h"
#include "medium.h"
#include "perfectelectricconductor.h"

namespace morfeus {
namespace media {

int32_t MediaLibrary::mAttribute = 0;

MediaLibrary::MediaLibrary()
  : core::MorfeusObject("MediaLibrary")
{
}

void MediaLibrary::addMedium(std::unique_ptr<Medium> medium)
{
  mMedia.push_back(std::move(medium));
}

Medium * MediaLibrary::medium(const std::string & name)
{
  for (std::size_t i = 0; i < mMedia.size(); i++) {
    Medium * m = mMedia.at(i).get();
    if (!m->name().empty() && m->name() == name) {
      return m;
    }
  }

  return nullptr;
}

const Medium * MediaLibrary::medium(const std::string & name) const
{
  for (std::size_t i = 0; i < mMedia.size(); i++) {
    const Medium * m = mMedia.at(i).get();
    if (!m->name().empty() && m->name() == name) {
      return m;
    }
  }

  return nullptr;
}

int32_t MediaLibrary::nextMediumAttrbute()
{
  return ++mAttribute;
}

std::size_t MediaLibrary::totalMediums() const
{
  return mMedia.size();
}

void MediaLibrary::doPrint(std::ostream & output, int tabPos) const
{
  for (std::size_t i = 0; i < mMedia.size(); i++) {
    const Medium * medium = mMedia.at(i).get();
    medium->print(output, tabPos+2);
  }
  output << "\n";
}

void MediaLibrary::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{

}

void MediaLibrary::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{

}

}
}
