#include "wire.h"

namespace morfeus {
namespace model {
namespace geometry {

int32_t Wire::mTotal = 0;
static const std::string OBJECT_ID("Wire");

Wire::Wire()
  : core::MorfeusObject (OBJECT_ID)
{
  init(nullptr, nullptr);
  setName(createName(mNumber));
}

Wire::Wire(const std::string & name)
  : core::MorfeusObject (OBJECT_ID, name)
{
  init(nullptr, nullptr);
}

Wire::Wire(const std::string & id, const std::string & name)
  : core::MorfeusObject (OBJECT_ID, id, name)
{
  init(nullptr, nullptr);
}

Wire::Wire(const Vertex * vertex1, const Vertex * vertex2)
  : core::MorfeusObject (OBJECT_ID)
{
  init(vertex1, vertex2);
  setName(createName(mNumber));
}

Wire::Wire(const std::string & name, const Vertex * vertex1, const Vertex * vertex2)
  : core::MorfeusObject (OBJECT_ID, name)
{
  init(vertex1, vertex2);
}

Wire::Wire(const std::string & id, const std::string & name, const Vertex * vertex1, const Vertex * vertex2)
  : core::MorfeusObject (OBJECT_ID, id, name)
{
  init(vertex1, vertex2);
}

void Wire::doPrint(std::ostream & output, int tabPos) const
{

}

void Wire::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{

}

void Wire::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{

}

void Wire::init(const Vertex *vertex1, const Vertex *vertex2)
{
  mNumber = mTotal++;
  mLocalMeshSize = -1.0;
  mVertex1 = vertex1;
  mVertex2 = vertex2;
}

}
}
}
