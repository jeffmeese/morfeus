#include "ellipse.h"

namespace morfeus {
namespace geometry {

static const std::string OBJECT_ID("Ellipse");

Ellipse::Ellipse()
  : Surface (OBJECT_ID)

{
}

Ellipse::Ellipse(const std::string & name)
  : Surface(OBJECT_ID, name)
{

}

Ellipse::Ellipse(const std::string & id, const std::string & name)
  : Surface (OBJECT_ID, id, name)
{

}

std::vector<Face*> Ellipse::doGetFaceList() const
{
  std::vector<Face*> faceList;
  return faceList;
}

std::vector<Segment*> Ellipse::doGetSegmentList() const
{
  std::vector<Segment*> segmentList;
  return segmentList;
}

void Ellipse::doPrint(std::ostream & output, int tabPos) const
{
}

void Ellipse::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
}

void Ellipse::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
}

}
}
