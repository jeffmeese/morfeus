#include "materialdatabase.h"
#include "material.h"

MaterialDatabase::MaterialDatabase()
{
}

void MaterialDatabase::addMaterial(std::unique_ptr<Material> material)
{
  mMaterials.push_back(std::move(material));
}

void MaterialDatabase::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Materials");
  for (std::size_t i = 0; i < mMaterials.size(); i++) {
    const Material * material = mMaterials.at(i).get();
    xmlutils::printHeader(output, tabPos, "Material");
    material->print(output, tabPos+2);
  }
}

void MaterialDatabase::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  rapidxml::xml_node<> * materials = node->first_node("Materials", 0, false);
  if (materials != nullptr) {
    rapidxml::xml_node<> * materialNode = node->first_node("Material", 0, false);
    while (materialNode != nullptr) {
      std::string type = xmlutils::readAttribute<std::string>(materialNode, "type");

      try {
        std::unique_ptr<Material> material(Material::Factory::Instance().CreateObject(type));
        material->readFromXml(document, materialNode);
        addMaterial(std::move(material));
      }
      catch (Loki::DefaultFactoryError<std::string, Material>::Exception &) {
        std::ostringstream oss;
        oss << "Could not create shape with type " << type;
        return;
      }

      // Shape factory needed here
      materialNode = materialNode->next_sibling("Material", 0, false);
    }
  }
}

void MaterialDatabase::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * materialsNode = xmlutils::createNode(document, "Materials");
  for (std::size_t i = 0; i < mMaterials.size(); i++) {
    rapidxml::xml_node<> * childNode = xmlutils::createNode(document, "Material");
    const Material * material = mMaterials.at(i).get();
    material->writeToXml(document, childNode);
    materialsNode->append_node(childNode);
  }
  node->append_node(materialsNode);
}

std::unique_ptr<Material> MaterialDatabase::takeMaterial(std::size_t index)
{
  std::unique_ptr<Material> material(mMaterials.at(index).release());
  mMaterials.erase(mMaterials.begin() + index);
  return material;
}
