#include "polygon.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace geometry {

static const std::string OBJECT_ID("Polygon");

Polygon::Polygon()
  : Surface(OBJECT_ID)
{

}

Polygon::Polygon(const std::string & name)
  : Surface(OBJECT_ID, name)
{

}

Polygon::Polygon(const std::string & id, const std::string & name)
  : Surface(OBJECT_ID, id, name)
{

}

void Polygon::addPoint(double x, double y, double z)
{
  addPoint(Point(x, y, z));
}

void Polygon::addPoint(const Point & point)
{
  mPoints.push_back(point);
}

std::vector<Face*> Polygon::doGetFaceList() const
{
  std::vector<Face*> faceList;
  return faceList;
}

std::vector<Segment*> Polygon::doGetSegmentList() const
{
  std::vector<Segment*> segmentList;
  return segmentList;
}

//std::vector<Face> Polygon::doGetFacetList() const
//{
//  std::vector<Face> facets;
//  return facets;
//}

//std::vector<Segment> Polygon::doGetSegmentList() const
//{
//  std::vector<Segment> segments;
//  return segments;
//}

//std::vector<Vertex> Polygon::doGetVertexList() const
//{
//  std::vector<Vertex> vertexList;
//  return vertexList;
//}

void Polygon::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printHeader(output, tabPos, "Points");
  for (std::size_t i = 0; i < mPoints.size(); i++) {
    xmlutils::printValue(output, tabPos+2, "Point: ", mPoints.at(i));
  }
}

void Polygon::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  mPoints.clear();

  rapidxml::xml_node<> * pointsNode = node->first_node("Points", 0, false);
  if (pointsNode != nullptr) {
    rapidxml::xml_node<> * pointNode = pointsNode->first_node("Point", 0, false);
    while (pointNode != nullptr) {
      Point pt;
      pt.readFromXml(document, pointNode);
      mPoints.push_back(pt);
    }
  }
}

void Polygon::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  for (std::size_t i = 0; i < mPoints.size(); i++) {
    const Point & pt = mPoints.at(i);
    rapidxml::xml_node<> * pointNode = xmlutils::createNode(document, "point");
    pt.writeToXml(document, pointNode);
    node->append_node(pointNode);
  }
}

namespace  {
  const bool r = Part::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Polygon*>()));
}

}
}
