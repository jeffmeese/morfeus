#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include "core/morfeusobject.h"

#include <memory>
#include <vector>

namespace morfeus {
  namespace media {
    class Medium;
  }
}

namespace morfeus {
namespace media {

class MediaLibrary
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL MediaLibrary();

public:
  MORFEUS_LIB_DECL void addMedium(std::unique_ptr<Medium> medium);
  MORFEUS_LIB_DECL Medium * medium(std::size_t index);
  MORFEUS_LIB_DECL const Medium * medium(std::size_t index) const;
  MORFEUS_LIB_DECL Medium * medium(const std::string & name);
  MORFEUS_LIB_DECL const Medium * medium(const std::string & name) const;
  MORFEUS_LIB_DECL std::size_t totalMediums() const;

public:
  MORFEUS_LIB_DECL static int32_t nextMediumAttrbute();

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  typedef std::unique_ptr<Medium> MediumPtr;
  typedef std::vector<MediumPtr> MediumVector;

private:
  MediumVector mMedia;
  static int32_t mAttribute;
};

inline Medium * MediaLibrary::medium(std::size_t index)
{
  return mMedia.at(index).get();
}

inline const Medium * MediaLibrary::medium(std::size_t index) const
{
  return mMedia.at(index).get();
}

}
}

#endif // MEDIALIBRARY_H
