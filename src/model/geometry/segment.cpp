#include "segment.h"

namespace morfeus {
namespace model {
namespace geometry {

int32_t Segment::mTotal = 0;
static const std::string OBJECT_ID("Segment");

Segment::Segment()
  : MorfeusObject (OBJECT_ID)
{
  init(nullptr, nullptr);
  setName(createName(mNumber));
}

Segment::Segment(const std::string & name)
  : MorfeusObject (OBJECT_ID, name)
{
  init(nullptr, nullptr);
}

Segment::Segment(const std::string & id, const std::string & name)
  : MorfeusObject (OBJECT_ID, id, name)
{
  init(nullptr, nullptr);
}

Segment::Segment(const Vertex * vertex1, const Vertex * vertex2)
  : MorfeusObject (OBJECT_ID)
{
  init(vertex1, vertex2);
  setName(createName(mNumber));
}

Segment::Segment(const std::string & name, const Vertex * vertex1, const Vertex * vertex2)
  : MorfeusObject (OBJECT_ID, name)
{
  init(vertex1, vertex2);
}

Segment::Segment(const std::string & id, const std::string & name, const Vertex * vertex1, const Vertex * vertex2)
  : MorfeusObject (OBJECT_ID, id, name)
{
  init(vertex1, vertex2);
}

void Segment::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "Number: ", mNumber);
}

void Segment::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
}

void Segment::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "number", mNumber);
}

void Segment::init(const Vertex * vertex1, const Vertex * vertex2)
{
  mNumber = mTotal++;
  mBoundary = false;
  mVertex1 = vertex1;
  mVertex2 = vertex2;
}

}
}
}
