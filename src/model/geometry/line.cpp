#include "line.h"

#include "vertex.h"
#include "wire.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace model {
namespace geometry {

int32_t Line::mTotal = 0;
static const std::string OBJECT_ID("Line");

Line::Line()
  : Part(OBJECT_ID)
{
  init(Point(0.0,0.0,0.0), Point(0.0,0.0,0.0));
  setName(createName(mNumber));
}

Line::Line(const std::string & name)
  : Part(OBJECT_ID, name)
{
  init(Point(0.0,0.0,0.0), Point(0.0,0.0,0.0));
}

Line::Line(const std::string & id, const std::string & name)
  : Part (OBJECT_ID, id, name)
{
  init(Point(0.0,0.0,0.0), Point(0.0,0.0,0.0));
}

Line::Line(const Point & startPt, const Point & endPoint)
  : Part (OBJECT_ID)
{
  init(startPt, endPoint);
  setName(createName(mNumber));
}

Line::Line(const std::string & name, const Point & startPt, const Point & endPoint)
  : Part (OBJECT_ID, name)
{
  init(startPt, endPoint);
}

Line::Line(const std::string & id, const std::string & name, const Point & startPt, const Point & endPoint)
  : Part (OBJECT_ID, id, name)
{
  init(startPt, endPoint);
}

bool Line::doContainsPoint(const Point &pt) const
{
  return false;
}

std::vector<Face*> Line::doGetFaceList() const
{
  std::vector<Face*> faceList;
  return faceList;
}

std::vector<Region*> Line::doGetRegionList() const
{
  std::vector<Region*> regionList;
  return regionList;
}

std::vector<Segment*> Line::doGetSegmentList() const
{
  std::vector<Segment*> segmentList;
  return segmentList;
}

std::vector<Vertex*> Line::doGetVertexList() const
{
  std::vector<Vertex*> vertexList;

  mVertex1->setPosition(mStartPoint.x(), mStartPoint.y(), mStartPoint.z());
  mVertex2->setPosition(mEndPoint.x(), mEndPoint.y(), mEndPoint.z());

  return vertexList;
}

std::vector<Wire*> Line::doGetWireList() const
{
  std::vector<Wire*> wireList;

  mWire->setVertex1(mVertex1.get());
  mWire->setVertex2(mVertex2.get());

  wireList.push_back(mWire.get());
  return wireList;
}

void Line::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Start");
  xmlutils::printValue(output, tabPos+2, "x: ", mStartPoint.x());
  xmlutils::printValue(output, tabPos+2, "y: ", mStartPoint.y());
  xmlutils::printValue(output, tabPos+2, "z: ", mStartPoint.z());
  xmlutils::printHeader(output, tabPos, "End");
  xmlutils::printValue(output, tabPos+2, "x: ", mEndPoint.x());
  xmlutils::printValue(output, tabPos+2, "y: ", mEndPoint.y());
  xmlutils::printValue(output, tabPos+2, "z: ", mEndPoint.z());
}

void Line::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  rapidxml::xml_node<> * startNode = node->first_node("Start", 0, false);
  if (startNode != nullptr) {
    mStartPoint.readFromXml(document, startNode);
  }

  rapidxml::xml_node<> * endNode = node->first_node("End", 0, false);
  if (endNode != nullptr) {
    mEndPoint.readFromXml(document, endNode);
  }
}

void Line::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * startNode = xmlutils::createNode(document, "Start");
  mStartPoint.writeToXml(document, startNode);
  rapidxml::xml_node<> * endNode = xmlutils::createNode(document, "End");
  mEndPoint.writeToXml(document, endNode);
}

void Line::init(const Point &startPoint, const Point &endPoint)
{
  mNumber = mTotal++;

  mWire.reset(new Wire);
  mVertex1.reset(new Vertex);
  mVertex2.reset(new Vertex);

  mStartPoint = startPoint;
  mEndPoint = endPoint;
}

namespace  {
  const bool r = Part::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Line*>()));
}

}
}
}

