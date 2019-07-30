#include "model.h"

#include "geometry/hole.h"
#include "geometry/region.h"
#include "geometry/shape.h"
#include "geometry/segment.h"
#include "geometry/triangleshape.h"
#include "geometry/vertex.h"
#include "mesh/edge.h"
#include "mesh/mesh.h"
#include "mesh/node.h"

#include "xml/rapidxml.hpp"
#include "xml/xmlutils.h"

#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>

namespace morfeus {
namespace geometry {

Model::Model()
  : MorfeusObject ("Geometry")
{
}

Model::~Model()
{
}

void Model::addHole(std::unique_ptr<Hole> hole)
{
  mHoles.push_back(std::move(hole));
}

void Model::addRegion(std::unique_ptr<Region> region)
{
  mRegions.push_back(std::move(region));
}

void Model::addShape(std::unique_ptr<Shape> shape)
{
  mShapes.push_back(std::move(shape));
}

void Model::print(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Geometry");

  if (mShapes.size() > 0) {
    xmlutils::printHeader(output, tabPos+2, "Shapes");
    for (std::size_t i = 0; i < mShapes.size(); i++) {
      const Shape * shape = mShapes.at(i).get();
      shape->print(output, tabPos+4);
    }
  }

  if (mRegions.size() > 0) {
    xmlutils::printHeader(output, tabPos+2, "Regions");
    for (std::size_t i = 0; i < mRegions.size(); i++) {
      const Region * region = mRegions.at(i).get();
      region->print(output, tabPos+4);
    }
  }

  if (mHoles.size() > 0) {
    xmlutils::printHeader(output, tabPos+2, "Holes");
    for (std::size_t i = 0; i < mHoles.size(); i++) {
      const Hole * hole = mHoles.at(i).get();
      hole->print(output, tabPos+4);
    }
  }

  xmlutils::printHeader(output, tabPos, "End Geometry");
  output << "\n";
}

void Model::print(int tabPos) const
{
  print(std::cout, tabPos);
}

void Model::readFromXml(rapidxml::xml_document<> &document, rapidxml::xml_node<> *node)
{
  rapidxml::xml_node<> * shapesNode = node->first_node("Shapes", 0, false);
  if (shapesNode != nullptr) {
    rapidxml::xml_node<> * shapeNode = shapesNode->first_node("Shape", 0, false);
    while (shapeNode != nullptr) {
      std::string type = xmlutils::readAttribute<std::string>(shapeNode, "type");
      std::unique_ptr<Shape> shape(Shape::factory().createShape(type));
      if (shape != nullptr) {
        shape->readFromXml(document, shapeNode);
        addShape(std::move(shape));
      }
      else {
        std::ostringstream oss;
        oss << "Warning: Shape type " << type << " not recognized";
        std::cerr << oss.str() << "\n";
      }
      shapeNode = shapeNode->next_sibling("Shape", 0, false);
    }
  }

  rapidxml::xml_node<> * holesNode = node->first_node("Holes", 0, false);
  if (holesNode != nullptr) {
    rapidxml::xml_node<> * holeNode = holesNode->first_node("Hole", 0, false);
    while (holeNode != nullptr) {
      std::unique_ptr<Hole> hole(new Hole);
      hole->readFromXml(document, holeNode);
      addHole(std::move(hole));
      holeNode = holeNode->next_sibling("Hole", 0, false);
    }
  }

  rapidxml::xml_node<> * regionsNode = node->first_node("Regions", 0, false);
  if (regionsNode != nullptr) {
    rapidxml::xml_node<> * regionNode = regionsNode->first_node("Region", 0, false);
    while (regionNode != nullptr) {
      std::unique_ptr<Region> region(new Region);
      region->readFromXml(document, regionNode);
      addRegion(std::move(region));
      regionNode = regionNode->next_sibling("Region", 0, false);
    }
  }
}

void Model::writeToXml(rapidxml::xml_document<> &document, rapidxml::xml_node<> *node) const
{
  rapidxml::xml_node<> * shapesNode = xmlutils::createNode(document, "Shapes");
  for (std::size_t i = 0; i < mShapes.size(); i++) {
    rapidxml::xml_node<> * shapeNode = xmlutils::createNode(document, "Shape");
    const Shape * shape = mShapes.at(i).get();
    shape->writeToXml(document, shapeNode);
  }
  node->append_node(shapesNode);

  rapidxml::xml_node<> * regionsNode = xmlutils::createNode(document, "Regions");
  for (std::size_t i = 0; i < mRegions.size(); i++) {
    rapidxml::xml_node<> * regionNode = xmlutils::createNode(document, "Region");
    const Region * region = mRegions.at(i).get();
    region->writeToXml(document, regionNode);
  }
  node->append_node(regionsNode);

  rapidxml::xml_node<> * holesNode = xmlutils::createNode(document, "Holes");
  for (std::size_t i = 0; i < mHoles.size(); i++) {
    rapidxml::xml_node<> * holeNode = xmlutils::createNode(document, "Hole");
    const Hole * hole = mHoles.at(i).get();
    hole->writeToXml(document, holeNode);
  }
  node->append_node(holesNode);
}

}
}


