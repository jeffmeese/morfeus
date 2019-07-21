#include "geometry.h"

#include "edge.h"
#include "hole.h"
#include "mesh.h"
#include "node.h"
#include "region.h"
#include "shape.h"
#include "segment.h"
#include "triangleshape.h"
#include "vertex.h"

#include "rapidxml.hpp"
#include "xmlutils.h"

#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>

Geometry::Geometry()
{
}

Geometry::~Geometry()
{
}

void Geometry::addHole(std::unique_ptr<Hole> hole)
{
  mHoles.push_back(std::move(hole));
}

void Geometry::addRegion(std::unique_ptr<Region> region)
{
  mRegions.push_back(std::move(region));
}

void Geometry::addShape(std::unique_ptr<Shape> shape)
{
  mShapes.push_back(std::move(shape));
}

void Geometry::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Geometry");

  xmlutils::printHeader(output, tabPos+2, "Shapes");
  for (std::size_t i = 0; i < mShapes.size(); i++) {
    const Shape * shape = mShapes.at(i).get();
    shape->print(output, tabPos+4);
  }

  xmlutils::printHeader(output, tabPos+2, "Regions");
  for (std::size_t i = 0; i < mRegions.size(); i++) {
    const Region * region = mRegions.at(i).get();
    region->print(output, tabPos+4);
  }

  xmlutils::printHeader(output, tabPos+2, "Holes");
  for (std::size_t i = 0; i < mHoles.size(); i++) {
    const Hole * hole = mHoles.at(i).get();
    hole->print(output, tabPos+4);
  }
}

void Geometry::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  rapidxml::xml_node<> * shapesNode = node->first_node("Shapes", 0, false);
  if (shapesNode != nullptr) {
    rapidxml::xml_node<> * shapeNode = node->first_node("Shape", 0, false);
    while (shapeNode != nullptr) {
      std::string type = xmlutils::readAttribute<std::string>(shapeNode, "type");

      try {
        std::unique_ptr<Shape> shape(Shape::Factory::Instance().CreateObject(type));
        shape->readFromXml(document, shapeNode);
        addShape(std::move(shape));
      }
      catch (Loki::DefaultFactoryError<std::string, Shape>::Exception &) {
        std::ostringstream oss;
        oss << "Could not create shape with type " << type;
        return;
      }

      // Shape factory needed here
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

void Geometry::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * geomNode = xmlutils::createNode(document, "Geometry");

  rapidxml::xml_node<> * shapesNode = xmlutils::createNode(document, "Shapes");
  for (std::size_t i = 0; i < mShapes.size(); i++) {
    rapidxml::xml_node<> * childNode = xmlutils::createNode(document, "Shape");
    const Shape * shape = mShapes.at(i).get();
    shape->writeToXml(document, childNode);
    shapesNode->append_node(childNode);
  }
  geomNode->append_node(shapesNode);

  rapidxml::xml_node<> * regionsNode = xmlutils::createNode(document, "Regions");
  for (std::size_t i = 0; i < mRegions.size(); i++) {
    rapidxml::xml_node<> * childNode = xmlutils::createNode(document, "Region");
    const Region * region = mRegions.at(i).get();
    region->writeToXml(document, childNode);
    regionsNode->append_node(childNode);
  }
  geomNode->append_node(regionsNode);

  rapidxml::xml_node<> * holesNode = xmlutils::createNode(document, "Holes");
  for (std::size_t i = 0; i < mHoles.size(); i++) {
    rapidxml::xml_node<> * childNode = xmlutils::createNode(document, "Hole");
    const Hole * hole = mHoles.at(i).get();
    hole->writeToXml(document, childNode);
    holesNode->append_node(childNode);
  }
  geomNode->append_node(holesNode);

  node->append_node(geomNode);
}


