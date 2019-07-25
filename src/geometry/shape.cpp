#include "shape.h"

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

std::vector<MesherPolygon> Shape::getMesherPolygons() const
{
  return doGetMesherPolygons();
}

void Shape::print(std::ostream & output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "Name: ", mName);
  doPrint(output, tabPos);
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
  rapidxml::xml_node<> * childNode = xmlutils::createNode(document, "Shape");
  xmlutils::writeAttribute(document, childNode, "name", mName);
  doXmlWrite(document, childNode);
  node->append_node(childNode);
}

std::ostream & operator<<(std::ostream & output, const Shape & object)
{
  object.print(output);
  return output;
}


