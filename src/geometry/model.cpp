#include "model.h"

#include "geometry/part.h"

#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>

namespace morfeus {
namespace geometry {

Model::Model()
  : MorfeusObject ("Model")
{
}

Model::~Model()
{
}

void Model::addPart(std::unique_ptr<Part> part)
{
  mParts.push_back(std::move(part));
}

void Model::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Model");

  if (mParts.size() > 0) {
    xmlutils::printHeader(output, tabPos+2, "Parts");
    for (std::size_t i = 0; i < mParts.size(); i++) {
      const Part * shape = mParts.at(i).get();
      shape->print(output, tabPos+4);
    }
  }

  xmlutils::printHeader(output, tabPos, "End Model");
  output << "\n";
}

void Model::doXmlRead(rapidxml::xml_document<> &document, rapidxml::xml_node<> *node)
{
  rapidxml::xml_node<> * partsNode = node->first_node("Parts", 0, false);
  if (partsNode != nullptr) {
    rapidxml::xml_node<> * partNode = partsNode->first_node("Part", 0, false);
    while (partNode != nullptr) {
      std::string type = xmlutils::readAttribute<std::string>(partNode, "type");
      std::unique_ptr<Part> part(Part::factory().create(type));
      if (part != nullptr) {
        part->readFromXml(document, partNode);
        addPart(std::move(part));
      }
      else {
        std::ostringstream oss;
        oss << "Warning: Part type " << type << " not recognized";
        std::cerr << oss.str() << "\n";
      }
      partNode = partNode->next_sibling("Part", 0, false);
    }
  }
}

void Model::doXmlWrite(rapidxml::xml_document<> &document, rapidxml::xml_node<> *node) const
{
  rapidxml::xml_node<> * partsNode = xmlutils::createNode(document, "Parts");
  for (std::size_t i = 0; i < mParts.size(); i++) {
    rapidxml::xml_node<> * partNode = xmlutils::createNode(document, "Part");
    const Part * part = mParts.at(i).get();
    part->writeToXml(document, partNode);
  }
  node->append_node(partsNode);
}

}
}


