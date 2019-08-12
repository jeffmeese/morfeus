#include "cylinder.h"

#include "face.h"
#include "facet.h"
#include "region.h"
#include "vertex.h"

#include "math/constants.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace model {
namespace geometry {

int32_t Cylinder::mTotal = 0;
static const std::string OBJECT_ID("Cylinder");

Cylinder::Cylinder()
  : Part(OBJECT_ID)
{
  init(Point(0.0, 0.0, 0.0), 0.5, 1.0);
  setName(createName(mNumber));
}

Cylinder::Cylinder(const std::string & name)
  : Part(OBJECT_ID, name)
{
  init(Point(0.0, 0.0, 0.0), 0.5, 1.0);
}

Cylinder::Cylinder(const std::string & id, const std::string & name)
  : Part(OBJECT_ID, id, name)
{
  init(Point(0.0, 0.0, 0.0), 0.5, 1.0);
  setName(createName(mNumber));
}

Cylinder::Cylinder(const Point & center, double radius, double height)
  : Part(OBJECT_ID)
{
  init(center, radius, height);
}

Cylinder::Cylinder(const std::string & name, const Point & center, double radius, double height)
  : Part(OBJECT_ID, name)
{
  init(center, radius, height);
}

Cylinder::Cylinder(const std::string & id, const std::string & name, const Point & center, double radius, double height)
  : Part(OBJECT_ID, id, name)
{
  init(center, radius, height);
}

bool Cylinder::doContainsPoint(const Point &pt) const
{
  return false;
}

std::vector<Face*> Cylinder::doGetFaceList() const
{
  std::vector<Face*> faceList;
  for (std::size_t i = 0; i < mFaceList.size(); i++) {
    faceList.push_back(mFaceList.at(i).get());
  }
  return faceList;
}

std::vector<Region*> Cylinder::doGetRegionList() const
{
  std::vector<Region*> regionList;
  regionList.push_back(mRegion.get());
  return regionList;
}

std::vector<Segment*> Cylinder::doGetSegmentList() const
{
  std::vector<Segment*> segmentList;
  return segmentList;
}

std::vector<Vertex*> Cylinder::doGetVertexList() const
{
  std::vector<Vertex*> vertexList;
  for (std::size_t i = 0; i < mVertexList.size(); i++) {
    vertexList.push_back(mVertexList.at(i).get());
  }
  return vertexList;
}

std::vector<Wire*> Cylinder::doGetWireList() const
{
  std::vector<Wire*> wireList;
  return wireList;
}

void Cylinder::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Center");
  mCenter.print(output, tabPos+2);
  xmlutils::printValue(output, tabPos, "radius: ", mRadius);
  xmlutils::printValue(output, tabPos, "height: ", mHeight);
  xmlutils::printValue(output, tabPos, "resolution: ", mResolution);
}

void Cylinder::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  rapidxml::xml_node<> * centerNode = node->first_node("Center", 0, false);
  if (centerNode != nullptr) {
    mCenter.readFromXml(document, centerNode);
  }
  setRadius(xmlutils::readAttribute<double>(node, "radius"));
  setHeight(xmlutils::readAttribute<double>(node, "height"));
  setResolution(xmlutils::readAttribute<std::size_t>(node, "resolution"));
}

void Cylinder::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "radius", mRadius);
  xmlutils::writeAttribute(document, node, "height", mHeight);
  xmlutils::writeAttribute(document, node, "resolution", mResolution);
  rapidxml::xml_node<> * centerNode = xmlutils::createNode(document, "Center");
  mCenter.writeToXml(document, centerNode);
}

void Cylinder::init(const Point &center, double radius, double height)
{
  mNumber = mTotal++;
  mCenter = center;
  mRadius = radius;
  mHeight = height;
  mResolution = 40;
  mRegion.reset(new Region);

  double angleIncrement = 360.0 / mResolution;
  std::size_t capVertices = mResolution;
  std::size_t capSegments = capVertices;
  std::size_t totalVertices = capVertices*2;
  std::size_t totalSegments = capSegments*3;

  // Create the vertices for the top face
  for (std::size_t i = 0; i < capVertices; i++) {
    VertexPtr v(new Vertex);
    v->setNumber(i);
    mVertexList.push_back(std::move(v));
  }

  // Create the vertices for the bottom face
  for (std::size_t i = 0; i < capVertices; i++) {
    VertexPtr v(new Vertex);
    mVertexList.push_back(std::move(v));
  }

  // Create the segments for the top face
  for (std::size_t i = 0; i < capSegments; i++) {
    SegmentPtr segment(new Segment);
    std::size_t index1 = i;
    std::size_t index2 = i+1;
    if (i == mVertexList.size()-1) {
      index2 = 0;
    }
    segment->setVertex1(mVertexList[index1].get());
    segment->setVertex2(mVertexList[index2].get());
    mSegmentList.push_back(std::move(segment));
  }

  // Create the segments for the bottom face
  for (std::size_t i = 0; i < capSegments; i++) {
    SegmentPtr segment(new Segment);
    std::size_t index1 = i + capVertices;
    std::size_t index2 = i + capVertices + 1;
    if (i == capSegments-1) {
      index2 = capVertices;
    }

    segment->setVertex1(mVertexList[index1].get());
    segment->setVertex2(mVertexList[index2].get());
    mSegmentList.push_back(std::move(segment));
  }

  // Create the top cap
  FacePtr topFace(new Face);
  std::unique_ptr<Facet> topFacet(new Facet);
  for (std::size_t i = 0; i < capVertices; i++) {
    topFacet->addVertex(mVertexList[i].get());
  }
  topFace->addFacet(std::move(topFacet));
  mFaceList.push_back(std::move(topFace));

  FacePtr bottomFace(new Face);
  std::unique_ptr<Facet> bottomFacet(new Facet);
  for (std::size_t i = 0; i < capVertices; i++) {
    bottomFacet->addVertex(mVertexList[i+capVertices].get());
  }
  bottomFace->addFacet(std::move(bottomFacet));
  mFaceList.push_back(std::move(bottomFace));

  FacePtr tubeFace(new Face);
  for (std::size_t i = 0; i < capVertices; i++) {
    std::size_t index1 = i;
    std::size_t index2 = capVertices+i;
    std::size_t index3 = capVertices+i+1;
    std::size_t index4 = i+1;
    if (i == capVertices-1) {
      index3 = capVertices;
      index4 = 0;
    }

    std::unique_ptr<Facet> facet(new Facet);
    facet->addVertex(mVertexList.at(index1).get());
    facet->addVertex(mVertexList.at(index2).get());
    facet->addVertex(mVertexList.at(index3).get());
    facet->addVertex(mVertexList.at(index4).get());
    tubeFace->addFacet(std::move(facet));
  }
  mFaceList.push_back(std::move(tubeFace));

  updatePosition();
}

void Cylinder::setCenter(const Point & pt)
{
  mCenter = pt;
  updatePosition();
}

void Cylinder::setHeight(double value)
{
  mHeight = value;
  updatePosition();
}

void Cylinder::setRadius(double value)
{
  mRadius = value;
  updatePosition();
}

void Cylinder::setResolution(std::size_t value)
{
  mResolution = value;
  updatePosition();
}

void Cylinder::updatePosition()
{
  double angleIncrement = 360.0 / mResolution;
  for (std::size_t i = 0; i < mResolution; i++) {
    double angle = i*angleIncrement * math::deg2rad;
    double x = mRadius * cos(angle);
    double y = mRadius * sin(angle);
    double z = mCenter.z();
    mVertexList.at(i)->setPosition(x, y, z);
  }

  for (std::size_t i = 0; i < mResolution; i++) {
    double angle = i*angleIncrement * math::deg2rad;
    double x = mRadius * cos(angle);
    double y = mRadius * sin(angle);
    double z = mCenter.z() + mHeight;
    mVertexList.at(i+mResolution)->setPosition(x, y, z);
  }
}

namespace  {
  const bool r = Part::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Cylinder*>()));
}

}
}
}
