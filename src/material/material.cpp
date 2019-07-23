#include "material.h"

Material::Material(const std::string & type)
  : MorfeusObject (type)
{
  init();
}

Material::Material(const std::string & type, const std::string & name)
  : MorfeusObject (type)
  , mName(name)
{
  init();
}

Material::Material(const std::string & type, const std::string & id, const std::string & name)
  : MorfeusObject(type, id)
  , mName(name)
{
  init();
}

Material::MaterialFactory & Material::factory()
{
  static MaterialFactory f;
  return f;
}

bool Material::MaterialFactory::registerType(const std::string &type, boost::function<Material *()> creator)
{
  mFactory.registerFactory(type, creator);
  return true;
}

Material * Material::MaterialFactory::create(const std::string & type)
{
  return mFactory.create(type);
}

void Material::print(std::ostream & output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "Name: ", mName);
  doPrint(output, tabPos);
}

void Material::print(int tabPos) const
{
  print(std::cout);
}

void Material::readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setName(xmlutils::readAttribute<std::string>(node, "name"));
  doXmlRead(document, node);
}

void Material::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * childNode = xmlutils::createNode(document, "Material");
  xmlutils::writeAttribute(document, childNode, "name", mName);
  doXmlWrite(document, childNode);
  node->append_node(childNode);
}

void Material::init()
{
  for (std::size_t i = 0; i < 9; i++) {
    mValues[i] = dcomplex(1.0, 0.0);
  }
}
