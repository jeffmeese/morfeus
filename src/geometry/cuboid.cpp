#include "cuboid.h"

#include "face.h"
#include "region.h"
#include "segment.h"
#include "vertex.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace geometry {

static const std::string OBJECT_ID("Cuboid");

Cuboid::Cuboid()
  : Solid(OBJECT_ID)
{
  Point pt(0.0, 0.0, 0.0);
  init(pt, 1.0, 1.0, 1.0);
  createFaces();
  createRegions();
  updateFaces();
}

Cuboid::Cuboid(const std::string & name)
  : Solid(OBJECT_ID, name)
{
  Point pt(0.0, 0.0, 0.0);
  init(pt, 1.0, 1.0, 1.0);
  createFaces();
  createRegions();
  updateFaces();
}

Cuboid::Cuboid(const std::string & id, const std::string & name)
  : Solid(OBJECT_ID, id, name)
{
  Point pt(0.0, 0.0, 0.0);
  init(pt, 1.0, 1.0, 1.0);
  createFaces();
  createRegions();
  updateFaces();
}

Cuboid::Cuboid(const Point & pt, double length, double width, double height)
  : Solid(OBJECT_ID)
{
  init(pt, length, width, height);
  createFaces();
  createRegions();
  updateFaces();
}

Cuboid::Cuboid(const std::string & name, const Point & pt, double length, double width, double height)
  : Solid(OBJECT_ID, name)
{
  init(pt, length, width, height);
  createFaces();
  createRegions();
  updateFaces();
}

Cuboid::Cuboid(const std::string & id, const std::string & name, const Point & pt, double length, double width, double height)
  : Solid(OBJECT_ID, id, name)
{
  init(pt, length, width, height);
  createFaces();
  createRegions();
  updateFaces();
}

std::vector<Face*> Cuboid::doGetFaceList() const
{
  std::vector<Face*> faceList;
  return faceList;
}

std::vector<Region*> Cuboid::doGetRegionList() const
{
  std::vector<Region*> regionList;
  regionList.push_back(mRegions.at(0).get());
  return regionList;
}

std::vector<Segment*> Cuboid::doGetSegmentList() const
{
  std::vector<Segment*> segmentList;
  return segmentList;
}

void Cuboid::doPrint(std::ostream & output, int tabPos) const
{
}

void Cuboid::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
}

void Cuboid::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
}

void Cuboid::createFaces()
{
  mFaces.push_back(std::unique_ptr<Face>(new Face("Face 1")));
  mFaces.push_back(std::unique_ptr<Face>(new Face("Face 2")));
  mFaces.push_back(std::unique_ptr<Face>(new Face("Face 3")));
  mFaces.push_back(std::unique_ptr<Face>(new Face("Face 4")));
  mFaces.push_back(std::unique_ptr<Face>(new Face("Face 5")));
  mFaces.push_back(std::unique_ptr<Face>(new Face("Face 6")));
}

void Cuboid::createRegions()
{
  mRegions.push_back(RegionPtr(new Region("Region 1")));
}

void Cuboid::init(const Point &pt, double length, double width, double height)
{
  mBaseCenter = pt;
  mLength = length;
  mWidth = width;
  mHeight = height;
}

void Cuboid::updateFaces()
{

}

namespace  {
  const bool r = Part::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Cuboid*>()));
}

}
}
