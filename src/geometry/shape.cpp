#include "shape.h"

namespace morfeus {
namespace geometry {

Shape::Shape(const std::string & type)
  : MorfeusObject (type)
{
}

Shape::Shape(const std::string & type, const std::string & name)
  : MorfeusObject (type)
  , mName(name)
{
}

Shape::Shape(const std::string & type, const std::string & id, const std::string & name)
  : MorfeusObject (type, id)
  , mName(name)
{
}

Shape::ShapeFactory & Shape::factory()
{
  static ShapeFactory f;
  return f;
}

std::vector<Face> Shape::getFacetList() const
{
  return doGetFacetList();
}

std::vector<Segment> Shape::getSegmentList() const
{
  return doGetSegmentList();
}

std::vector<Vertex> Shape::getVertexList() const
{
  return doGetVertexList();
}

void Shape::print(std::ostream & output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "Type: ", type());
  xmlutils::printValue(output, tabPos, "Name: ", mName);
  doPrint(output, tabPos+2);
}

void Shape::print(int tabPos) const
{
  print(std::cout);
}

void Shape::readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  doXmlRead(document, node);
}

void Shape::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", type());
  xmlutils::writeAttribute(document, node, "name", mName);
  doXmlWrite(document, node);
}

std::ostream & operator<<(std::ostream & output, const Shape & object)
{
  object.print(output);
  return output;
}

}
}
