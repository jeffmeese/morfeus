#include "rectangle.h"

#include "face.h"
#include "model.h"
#include "segment.h"
#include "vertex.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace geometry {

static const std::string OBJECT_ID("Rectangle");

Rectangle::Rectangle()
  : Surface (OBJECT_ID)
{
  setCenter(Point(0.0, 0.0, 0.0));
  setLength(1.0);
  setWidth(1.0);
  mFaces.push_back(std::unique_ptr<Face>(new Face));
}

Rectangle::Rectangle(const std::string & name)
  : Surface (OBJECT_ID, name)
{
  setCenter(Point(0.0, 0.0, 0.0));
  setLength(1.0);
  setWidth(1.0);
   mFaces.push_back(std::unique_ptr<Face>(new Face));
}

Rectangle::Rectangle(const std::string & id, const std::string & name)
  : Surface (OBJECT_ID, id, name)
{
  setCenter(Point(0.0, 0.0, 0.0));
  setLength(1.0);
  setWidth(1.0);
   mFaces.push_back(std::unique_ptr<Face>(new Face));
}

Rectangle::Rectangle(const Point & center, double length, double width)
  : Surface (OBJECT_ID)
{
  setCenter(center);
  setLength(length);
  setWidth(width);
   mFaces.push_back(std::unique_ptr<Face>(new Face));
}

Rectangle::Rectangle(const std::string & name, const Point & center, double length, double width)
  : Surface (OBJECT_ID, name)
{
  setCenter(center);
  setLength(length);
  setWidth(width);
   mFaces.push_back(std::unique_ptr<Face>(new Face));
}

Rectangle::Rectangle(const std::string & id, const std::string & name, const Point & center, double length, double width)
  : Surface (OBJECT_ID, id, name)
{
  setCenter(center);
  setLength(length);
  setWidth(width);
   mFaces.push_back(std::unique_ptr<Face>(new Face));
}

std::vector<Face*> Rectangle::doGetFaceList() const
{
  std::vector<Face*> faceList;
  faceList.push_back(mFaces.at(0).get());
  return faceList;
}

std::vector<Segment*> Rectangle::doGetSegmentList() const
{
  std::vector<Segment*> segmentList;
  return segmentList;
}

void Rectangle::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
  //xmlutils::printValue(output, tabPos, "xl: ", mXl);
  //xmlutils::printValue(output, tabPos, "xu: ", mXu);
  //xmlutils::printValue(output, tabPos, "yl: ", mYl);
  //xmlutils::printValue(output, tabPos, "yu: ", mYu);
  //xmlutils::printValue(output, tabPos, "z: ", mZ);
}

void Rectangle::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
//  setXl(xmlutils::readAttribute<double>(node, "xl"));
//  setXu(xmlutils::readAttribute<double>(node, "xu"));
//  setYl(xmlutils::readAttribute<double>(node, "yl"));
//  setYu(xmlutils::readAttribute<double>(node, "yu"));
//  setZ(xmlutils::readAttribute<double>(node, "z"));
}

void Rectangle::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
//  xmlutils::writeAttribute(document, node, "xl", mXl);
//  xmlutils::writeAttribute(document, node, "xu", mXu);
//  xmlutils::writeAttribute(document, node, "yl", mYl);
//  xmlutils::writeAttribute(document, node, "yu", mYu);
//  xmlutils::writeAttribute(document, node, "z", mZ);
}


namespace  {
  const bool r = Part::factory().registerType(OBJECT_ID, boost::bind(boost::factory<Rectangle*>()));
}

}
}
