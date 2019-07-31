#include "face.h"

namespace morfeus {
namespace geometry {

static const std::string OBJECT_ID("Face");

Face::Face()
  : MorfeusObject(OBJECT_ID)
  , mMedium(nullptr)
{
}

Face::Face(const std::string & name)
  : MorfeusObject(OBJECT_ID)
  , mName(name)
  , mMedium(nullptr)
{
}

Face::Face(const std::string & id, const std::string & name)
  : MorfeusObject(OBJECT_ID, id)
  , mName(name)
  , mMedium(nullptr)
{
}

void Face::doPrint(std::ostream & output, int tabPos) const
{
}

void Face::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
}

void Face::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
}

}
}
