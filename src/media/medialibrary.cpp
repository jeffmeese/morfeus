#include "medialibrary.h"

#include "medium.h"

namespace morfeus {
namespace media {

MediaLibrary::MediaLibrary()
  : core::MorfeusObject("MediaLibrary")
{

}

void MediaLibrary::addMedium(std::unique_ptr<Medium> medium)
{
  mMedia.push_back(std::move(medium));
}

Medium * MediaLibrary::medium(std::size_t index)
{
  return mMedia.at(index).get();
}

const Medium * MediaLibrary::medium(std::size_t index) const
{
  return mMedia.at(index).get();
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

std::size_t MediaLibrary::totalMediums() const
{
  return mMedia.size();
}

void MediaLibrary::doPrint(std::ostream & output, int tabPos) const
{
}

void MediaLibrary::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{

}

void MediaLibrary::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{

}

}
}
