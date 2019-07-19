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

void Geometry::addRegion(std::unique_ptr<Region> region)
{
  mRegions.push_back(std::move(region));
}

void Geometry::addSegment(std::unique_ptr<Segment> segment)
{
  mSegments.push_back(std::move(segment));
}

void Geometry::addShape(std::unique_ptr<Shape> shape)
{
  mShapes.push_back(std::move(shape));
}

void Geometry::addVertex(std::unique_ptr<Vertex> vertex)
{
  mVertices.push_back(std::move(vertex));
}

void Geometry::doReadFromXml(ptree &tree)
{

}

void Geometry::doWriteToXml(ptree &item) const
{
  ptree geometryTree;
  for (std::size_t i = 0; i < mShapes.size(); i++) {
    const Shape * shape = mShapes.at(i).get();
    shape->writeToXml(geometryTree);
  }
  for (std::size_t i = 0; i < mRegions.size(); i++) {
    const Region * region = mRegions.at(i).get();
    region->writeToXml(geometryTree);
  }
  item.add_child("Geometry", geometryTree);
}

void Geometry::readPolyFile(const std::string & fileName)
{
  std::ifstream inputFile(fileName.c_str());
  if (inputFile.fail()) {
    std::ostringstream oss;
    oss << "Could not open poly file " << fileName;
    throw std::invalid_argument(oss.str());
  }

  int32_t numRegions = readPolyVertices(inputFile);
  readPolySegments(inputFile);
  readPolyHoles(inputFile);
  if (numRegions > 0) {
    readPolyRegions(inputFile, numRegions);
  }
}

void Geometry::readPolyHoles(std::ifstream & inputFile)
{
  std::string inputLine;
  std::getline(inputFile, inputLine);

  int32_t numHoles = 0;
  std::istringstream iss(inputLine);
  iss >> numHoles;
  for (int32_t i = 0; i < numHoles; i++) {
    std::getline(inputFile, inputLine);

    int32_t id = 0;
    double x = 0.0, y = 0.0;
    std::istringstream iss(inputLine);
    iss >> id >> x >> y;

    std::unique_ptr<Hole> hole(new Hole(x, y));
    hole->setNumber(id);
    addShape(std::move(hole));
  }
}

void Geometry::readPolyRegions(std::ifstream & inputFile, int32_t numRegions)
{
  std::string inputLine;
  for (int32_t i = 0; i < numRegions; i++) {
    std::getline(inputFile, inputLine);

    int32_t id;
    double x = 0.0, y = 0.0, attribute = 0.0, maxArea = 0.0;
    std::istringstream iss(inputLine);
    iss >> id >> x >> y >> attribute >> maxArea;

    RegionPtr region(new Region(x, y));
    region->setNumber(id);
    region->setAttribute(attribute);
    region->setMaxArea(maxArea);

    addRegion(std::move(region));
  }
}

void Geometry::readPolySegments(std::ifstream & inputFile)
{
  std::string inputLine;

  // Read the segment header line
  std::getline(inputFile, inputLine);
  int32_t numSegments;

  std::istringstream iss(inputLine);
  iss >> numSegments;
  for (int32_t i = 0; i < numSegments; i++) {
    std::getline(inputFile, inputLine);

    int32_t id, node1, node2;
    std::istringstream iss(inputLine);
    iss >> id >> node1 >> node2;

    SegmentPtr segment(new Segment(node1, node2));
    segment->setNumber(id);
    int32_t boundary;
    iss >> boundary;
    if (!iss.fail()) {
      segment->setBoundary(boundary == 1);
    }
    addSegment(std::move(segment));
  }
}

int32_t Geometry::readPolyVertices(std::ifstream & inputFile)
{
  // Read the vertex header line
  std::string inputLine;
  std::getline(inputFile, inputLine);
  int numVertices, dimension, numAttrs, numMarkers;
  std::istringstream iss(inputLine);
  iss >> numVertices >> dimension >> numAttrs >> numMarkers;

  // Read the vertices
  for (int32_t i = 0; i < numVertices; i++) {
    std::getline(inputFile, inputLine);

    int32_t id;
    double x, y;
    std::istringstream iss(inputLine);
    iss >> id >> x >> y;

    VertexPtr vertex(new Vertex(x, y));
    vertex->setNumber(id);
    if (numAttrs > 0) {
      for (int32_t j = 0; j < numAttrs; j++) {
        double value;
        iss >> value;
        vertex->addAttribute(value);
      }
    }

    if (numMarkers == 1) {
      int32_t value;
      iss >> value;
      vertex->setBoundary(value == 1);
    }
    addVertex(std::move(vertex));
  }

  return numAttrs;
}

