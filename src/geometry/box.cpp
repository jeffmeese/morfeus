#include "box.h"

static const std::string OBJECT_ID("Box");

Box::Box()
  : Shape(OBJECT_ID)
{

}

Box::Box(const std::string & name)
  : Shape(OBJECT_ID, name)
{

}

std::vector<MesherPolygon> Box::doGetMesherPolygons() const
{
  std::vector<MesherPolygon> polygons;
  return polygons;
}

void Box::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "xl: ", mXl);
  xmlutils::printValue(output, tabPos, "xu: ", mXu);
  xmlutils::printValue(output, tabPos, "yl: ", mYl);
  xmlutils::printValue(output, tabPos, "yu: ", mYu);
  xmlutils::printValue(output, tabPos, "zl: ", mZl);
  xmlutils::printValue(output, tabPos, "zu: ", mZu);
}

void Box::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setXl(xmlutils::readAttribute<double>(node, "xl"));
  setXu(xmlutils::readAttribute<double>(node, "xu"));
  setYl(xmlutils::readAttribute<double>(node, "yl"));
  setYu(xmlutils::readAttribute<double>(node, "yu"));
  setZl(xmlutils::readAttribute<double>(node, "zl"));
  setZl(xmlutils::readAttribute<double>(node, "zu"));
}

void Box::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "xl", mXl);
  xmlutils::writeAttribute(document, node, "xu", mXu);
  xmlutils::writeAttribute(document, node, "yl", mYl);
  xmlutils::writeAttribute(document, node, "yu", mYu);
  xmlutils::writeAttribute(document, node, "yl", mZl);
  xmlutils::writeAttribute(document, node, "yu", mZu);
}
