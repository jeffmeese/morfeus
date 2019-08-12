#include "region.h"

#include <sstream>

namespace morfeus {
namespace model {
namespace geometry {

int32_t Region::mTotal = 0;
static const std::string OBJECT_ID("Region");

Region::Region()
  : MorfeusObject (OBJECT_ID)
{
  init(Point(0.0,0.0,0.0));
  setName(createName(mNumber));
}

Region::Region(const std::string & name)
  : MorfeusObject (OBJECT_ID, name)
{
  init(Point(0.0,0.0,0.0));
}

Region::Region(const std::string & id, const std::string & name)
  : MorfeusObject (OBJECT_ID, id, name)
{
  init(Point(0.0,0.0,0.0));
}

Region::Region(const Point & pt)
  : MorfeusObject (OBJECT_ID)
{
  init(pt);
  setName(createName(mNumber));
}

Region::Region(const std::string & name, const Point & pt)
  : MorfeusObject (OBJECT_ID, name)
{
  init(pt);
}

Region::Region(const std::string & id, const std::string & name, const Point & pt)
  : MorfeusObject (OBJECT_ID, id, name)
{
  init(pt);
}

void Region::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Position");
  mPosition.print(output, tabPos+2);
  xmlutils::printValue(output, tabPos, "Local Mesh Size: ", mLocalMeshSize);
}

void Region::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setLocalMeshSize(xmlutils::readAttribute<double>(node, "local-mesh-size"));
  rapidxml::xml_node<> * posNode = node->first_node("Position", 0, false);
  if (posNode != nullptr) {
    mPosition.readFromXml(document, posNode);
  }
}

void Region::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "local-mesh-size", mLocalMeshSize);
  rapidxml::xml_node<> * posNode = xmlutils::createNode(document, "Position");
  mPosition.writeToXml(document, posNode);
}

void Region::init(const Point &pt)
{
  mPosition = pt;
  mLocalMeshSize = -1.0;
  mMedium = nullptr;
  mNumber = mTotal++;
}

}
}
}
