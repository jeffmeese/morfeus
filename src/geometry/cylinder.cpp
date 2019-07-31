#include "cylinder.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace geometry {

static const std::string OBJECT_ID("Cylinder");

Cylinder::Cylinder()
  : Solid(OBJECT_ID)
{
  setCenter(Point(0.0,0.0,0.0));
  setRadius(0.5);
  setHeight(1.0);
  setResolution(6);
}

Cylinder::Cylinder(const std::string & name)
  : Solid(OBJECT_ID, name)
{
  setCenter(Point(0.0,0.0,0.0));
  setRadius(0.5);
  setHeight(1.0);
  setResolution(6);
}

Cylinder::Cylinder(const std::string & id, const std::string & name)
  : Solid(OBJECT_ID, id, name)
{
  setCenter(Point(0.0,0.0,0.0));
  setRadius(0.5);
  setHeight(1.0);
  setResolution(6);
}

Cylinder::Cylinder(const Point & center, double radius, double height)
  : Solid(OBJECT_ID)
{
  setCenter(center);
  setRadius(radius);
  setHeight(height);
  setResolution(6);
}

Cylinder::Cylinder(const std::string & name, const Point & center, double radius, double height)
  : Solid(OBJECT_ID, name)
{
  setCenter(center);
  setRadius(radius);
  setHeight(height);
  setResolution(6);
}

Cylinder::Cylinder(const std::string & id, const std::string & name, const Point & center, double radius, double height)
  : Solid(OBJECT_ID, id, name)
{
  setCenter(center);
  setRadius(radius);
  setHeight(height);
  setResolution(6);
}

std::vector<Face*> Cylinder::doGetFaceList() const
{
  std::vector<Face*> faceList;
  return faceList;
}

std::vector<Region*> Cylinder::doGetRegionList() const
{
  std::vector<Region*> regionList;
  return regionList;
}

std::vector<Segment*> Cylinder::doGetSegmentList() const
{
  std::vector<Segment*> segmentList;
  return segmentList;
}

void Cylinder::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  xmlutils::printValue(output, tabPos, "x: ", mCenter.x());
  xmlutils::printValue(output, tabPos, "y: ", mCenter.y());
  xmlutils::printValue(output, tabPos, "z: ", mCenter.z());
  xmlutils::printValue(output, tabPos, "radius: ", mRadius);
  xmlutils::printValue(output, tabPos, "height: ", mHeight);
  xmlutils::printValue(output, tabPos, "resolution: ", mResolution);
}

void Cylinder::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  double x = xmlutils::readAttribute<double>(node, "x");
  double y = xmlutils::readAttribute<double>(node, "y");
  double z = xmlutils::readAttribute<double>(node, "z");

  setCenter(Point(x, y, z));
  setRadius(xmlutils::readAttribute<double>(node, "radius"));
  setHeight(xmlutils::readAttribute<double>(node, "height"));
  setResolution(xmlutils::readAttribute<std::size_t>(node, "resolution"));
}

void Cylinder::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "radius", mRadius);
  xmlutils::writeAttribute(document, node, "height", mHeight);
  xmlutils::writeAttribute(document, node, "resolution", mResolution);
  rapidxml::xml_node<> * centerNode = xmlutils::createNode(document, "Center");
  mCenter.writeToXml(document, centerNode);
}

namespace  {
  const bool r = Part::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Cylinder*>()));
}

}
}
