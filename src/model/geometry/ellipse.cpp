#include "ellipse.h"

#include "face.h"
#include "facet.h"
#include "polygon.h"
#include "vertex.h"

#include "math/constants.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace model {
namespace geometry {

int32_t Ellipse::mTotal = 0;
static const std::string OBJECT_ID("Ellipse");

Ellipse::Ellipse()
  : Part (OBJECT_ID)
{
  init(Point(0.0, 0.0, 0.0), 1.0, 0.0);
  setName(createName(mNumber));
}

Ellipse::Ellipse(const std::string & name)
  : Part(OBJECT_ID, name)
{
  init(Point(0.0, 0.0, 0.0), 1.0, 0.0);
}

Ellipse::Ellipse(const std::string & id, const std::string & name)
  : Part (OBJECT_ID, id, name)
{
  init(Point(0.0, 0.0, 0.0), 1.0, 0.0);
}

Ellipse::Ellipse(const Point & pt, double uRadius, double vRadius)
  : Part (OBJECT_ID)
{
  init(pt, uRadius, vRadius);
  setName(createName(mNumber));
}

Ellipse::Ellipse(const std::string & name, const Point & pt, double uRadius, double vRadius)
  : Part (OBJECT_ID, name)
{
  init(pt, uRadius, vRadius);
}

Ellipse::Ellipse(const std::string & id, const std::string & name, const Point & pt, double uRadius, double vRadius)
  : Part (OBJECT_ID, id, name)
{
  init(pt, uRadius, vRadius);
}

bool Ellipse::doContainsPoint(const Point &pt) const
{
  return false;
}

std::vector<Face*> Ellipse::doGetFaceList() const
{
  std::vector<Face*> faceList;
  faceList.push_back(mFace.get());
  return faceList;
}

std::vector<Region*> Ellipse::doGetRegionList() const
{
  std::vector<Region*> regionList;
  return regionList;
}

std::vector<Segment*> Ellipse::doGetSegmentList() const
{
  std::vector<Segment*> segmentList;
  return segmentList;
}

std::vector<Vertex*> Ellipse::doGetVertexList() const
{
  std::vector<Vertex*> vertexList;
  return vertexList;
}

std::vector<Wire*> Ellipse::doGetWireList() const
{
  std::vector<Wire*> wireList;
  return wireList;
}

void Ellipse::doPrint(std::ostream & output, int tabPos) const
{
}

void Ellipse::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
}

void Ellipse::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
}

void Ellipse::init(const Point & pt, double uRadius, double vRadius)
{
  mNumber = mTotal++;
  mCenter = pt;
  mRadiusU = uRadius;
  mRadiusV = vRadius;
  mResolution = 6;

  // Create the vertices
  for (std::size_t i = 0; i < mResolution; i++) {
    VertexPtr v(new Vertex);
    v->setNumber(i);
    mVertexList.push_back(std::move(v));
  }

  // Create the segments
  for (std::size_t i = 0; i < mResolution; i++) {
    SegmentPtr segment(new Segment);
    std::size_t index1 = i;
    std::size_t index2 = i+1;
    if (i == mResolution-1) {
      index2 = 0;
    }
    segment->setVertex1(mVertexList.at(index1).get());
    segment->setVertex2(mVertexList.at(index2).get());

    mSegmentList.push_back(std::move(segment));
  }

  // Create the faces
  mFace.reset(new Face);
  std::unique_ptr<Facet> facet(new Facet);
  for (std::size_t i = 0; i < mVertexList.size(); i++) {
    facet->addVertex(mVertexList.at(i).get());
  }
  mFace->addFacet(std::move(facet));

  // Update the vertex positions
  updatePosition();
}

void Ellipse::setCenter(const Point & pt)
{
  mCenter = pt;
  updatePosition();
}

void Ellipse::setResolution(std::size_t value)
{
  mResolution = value;
  updatePosition();
}

void Ellipse::setURadius(double value)
{
  mRadiusU = value;
  updatePosition();
}

void Ellipse::setVRadius(double value)
{
  mRadiusV = value;
  updatePosition();
}

void Ellipse::updatePosition()
{
  double angleIncrement = 360.0 / mResolution;
  for (std::size_t i = 0; i < mResolution; i++) {
    double angle = i*angleIncrement * math::deg2rad;
    double x = mRadiusU * cos(angle);
    double y = mRadiusV * sin(angle);
    double z = mCenter.z();
    mVertexList.at(i)->setPosition(x, y, z);
  }
}

namespace  {
  const bool r = Part::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Ellipse*>()));
}

}
}
}
