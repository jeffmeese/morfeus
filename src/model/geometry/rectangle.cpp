#include "rectangle.h"

#include "face.h"
#include "facet.h"
#include "segment.h"
#include "vertex.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace model {
namespace geometry {

int32_t Rectangle::mTotal = 0;
static const std::string OBJECT_ID("Rectangle");

Rectangle::Rectangle()
  : Part (OBJECT_ID)
{
  init(Point(0.0,0.0,0.0), 1.0, 1.0);
  setName(createName(mNumber));
}

Rectangle::Rectangle(const std::string & name)
  : Part (OBJECT_ID, name)
{
  init(Point(0.0,0.0,0.0), 1.0, 1.0);
}

Rectangle::Rectangle(const std::string & id, const std::string & name)
  : Part (OBJECT_ID, id, name)
{
  init(Point(0.0,0.0,0.0), 1.0, 1.0);
}

Rectangle::Rectangle(const Point & center, double length, double width)
  : Part (OBJECT_ID)
{
  init(center, length, width);
  setName(createName(mNumber));
}

Rectangle::Rectangle(const std::string & name, const Point & center, double length, double width)
  : Part (OBJECT_ID, name)
{
  init(center, length, width);
}

Rectangle::Rectangle(const std::string & id, const std::string & name, const Point & center, double length, double width)
  : Part (OBJECT_ID, id, name)
{
  init(center, length, width);
}

bool Rectangle::doContainsPoint(const Point &pt) const
{
  double x = pt.x();
  double y = pt.y();
  double z = pt.z();

  double xl = mCenter.x() - mWidth / 2;
  double xu = mCenter.x() + mWidth / 2;
  double yl = mCenter.y() - mLength / 2;
  double yu = mCenter.y() + mLength / 2;
  double zl = mCenter.z();

  return ( (x >= xl && x <= xu) && (y >= yl && y <= yu) && (fabs(zl-z) <= 1e-8) );
}

std::vector<Face*> Rectangle::doGetFaceList() const
{
  std::vector<Face*> faceList;
  faceList.push_back(mFace.get());
  return faceList;
}

std::vector<Region*> Rectangle::doGetRegionList() const
{
  std::vector<Region*> regionList;
  return regionList;
}

std::vector<Segment*> Rectangle::doGetSegmentList() const
{
  std::vector<Segment*> segmentList;
  return segmentList;
}

std::vector<Vertex*> Rectangle::doGetVertexList() const
{
  std::vector<Vertex*> vertexList;
  for (std::size_t i = 0; i < mVertices.size(); i++) {
    vertexList.push_back(mVertices.at(i).get());
  }
  return vertexList;
}

std::vector<Wire*> Rectangle::doGetWireList() const
{
  std::vector<Wire*> wireList;
  return wireList;
}

void Rectangle::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Center");
  mCenter.print(output, tabPos+2);
  xmlutils::printValue(output, tabPos, "Length: ", mLength);
  xmlutils::printValue(output, tabPos, "Width: ", mWidth);
}

void Rectangle::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  rapidxml::xml_node<> * centerNode = node->first_node("Center", 0, false);
  if (centerNode != nullptr) {
    mCenter.readFromXml(document, centerNode);
  }
  setLength(xmlutils::readAttribute<double>(node, "length"));
  setWidth(xmlutils::readAttribute<double>(node, "width"));
}

void Rectangle::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "length", mLength);
  xmlutils::writeAttribute(document, node, "width", mWidth);
  rapidxml::xml_node<> * centerNode = xmlutils::createNode(document, "Center");
  mCenter.writeToXml(document, centerNode);
}

void Rectangle::init(const Point &pt, double length, double width)
{
  mNumber = mTotal++;
  mCenter = pt;
  mLength = length;
  mWidth = width;

  for (std::size_t i = 0; i < 4; i++) {
    VertexPtr v(new Vertex);
    v->setNumber(i);
    mVertices.push_back(std::move(v));
  }

  std::unique_ptr<Facet> facet(new Facet);
  for (std::size_t i = 0; i < 4; i++) {
    facet->addVertex(mVertices.at(i).get());
  }

  mFace.reset(new Face);
  mFace->addFacet(std::move(facet));

  updatePosition();
}

void Rectangle::setCenter(Point value)
{
  mCenter = value;
  updatePosition();
}

void Rectangle::setLength(double value)
{
  mLength = value;
  updatePosition();
}

void Rectangle::setWidth(double value)
{
  mWidth = value;
  updatePosition();
}

void Rectangle::updatePosition()
{
  double xl = mCenter.x() - mWidth / 2;
  double xu = mCenter.x() + mWidth / 2;
  double yl = mCenter.y() - mLength / 2;
  double yu = mCenter.y() + mLength / 2;
  double z = mCenter.z();

  mVertices[0]->setPosition(xl, yl, z);
  mVertices[1]->setPosition(xu, yl, z);
  mVertices[2]->setPosition(xu, yu, z);
  mVertices[3]->setPosition(xl, yu, z);
}

namespace  {
  const bool r = Part::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Rectangle*>()));
}

}
}
}
