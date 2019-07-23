#include "materialdatabase.h"
#include "material.h"

static const std::string OBJECT_ID("Material Database");

MaterialDatabase::MaterialDatabase()
  : MorfeusObject (OBJECT_ID)
{
}

MaterialDatabase::MaterialDatabase(const std::string & id)
  : MorfeusObject (OBJECT_ID, id)
{
}

void MaterialDatabase::addMaterial(std::unique_ptr<Material> material)
{
  mMaterials.push_back(std::move(material));
}

void MaterialDatabase::print(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Materials");
  for (std::size_t i = 0; i < mMaterials.size(); i++) {
    const Material * material = mMaterials.at(i).get();
    xmlutils::printHeader(output, tabPos+2, "Material");
    material->print(output, tabPos+4);
  }
  xmlutils::printHeader(output, tabPos, "End Materials");
  output << "\n";
}

void MaterialDatabase::print(int tabPos) const
{
  print(std::cout, tabPos);
}

void MaterialDatabase::readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  rapidxml::xml_node<> * materialNode = node->first_node("Material", 0, false);
  while (materialNode != nullptr) {
    std::string type = xmlutils::readAttribute<std::string>(materialNode, "type");
    std::unique_ptr<Material> material(Material::factory().create(type));
    if (material != nullptr) {
      material->readFromXml(document, materialNode);
      addMaterial(std::move(material));
    }
    else {
      std::ostringstream oss;
      oss << "Warning: Material type " << type << " not recognized";
      std::cerr << oss.str() << "\n";
    }
    materialNode = materialNode->next_sibling("Material", 0, false);
  }
}

void MaterialDatabase::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * materialsNode = xmlutils::createNode(document, "Materials");
  for (std::size_t i = 0; i < mMaterials.size(); i++) {
    const Material * material = mMaterials.at(i).get();
    material->writeToXml(document, materialsNode);
  }
  node->append_node(materialsNode);
}

std::unique_ptr<Material> MaterialDatabase::takeMaterial(std::size_t index)
{
  std::unique_ptr<Material> material(mMaterials.at(index).release());
  mMaterials.erase(mMaterials.begin() + index);
  return material;
}
