#include "point.h"

namespace morfeus {
namespace model {
namespace geometry {

int32_t Point::mTotal = 0;
static const std::string OBJECT_ID("Point");

Point::Point()
  : core::MorfeusObject (OBJECT_ID)
{
  init(0.0, 0.0, 0.0);
}

Point::Point(const std::string & name)
  : core::MorfeusObject (OBJECT_ID, name)
{
  init(0.0, 0.0, 0.0);
}

Point::Point(const std::string & id, const std::string & name)
  : core::MorfeusObject (OBJECT_ID, id, name)
{
  init(0.0, 0.0, 0.0);
}

Point::Point(double x, double y, double z)
  : core::MorfeusObject (OBJECT_ID)
{
  init(x, y, z);
}

Point::Point(const std::string & name, double x, double y, double z)
  : core::MorfeusObject (OBJECT_ID, name)
{
  init(x, y, z);
}

Point::Point(const std::string & id, const std::string & name, double x, double y, double z)
  : core::MorfeusObject (OBJECT_ID, id, name)
{
  init(x, y, z);
}

void Point::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "x : ", mX);
  xmlutils::printValue(output, tabPos, "y : ", mY);
  xmlutils::printValue(output, tabPos, "z : ", mZ);
}

void Point::doXmlRead(rapidxml::xml_document<> &document, rapidxml::xml_node<> *node)
{
  setX(xmlutils::readAttribute<double>(node, "x"));
  setY(xmlutils::readAttribute<double>(node, "y"));
  setZ(xmlutils::readAttribute<double>(node, "z"));
}

void Point::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "x", mX);
  xmlutils::writeAttribute(document, node, "y", mY);
  xmlutils::writeAttribute(document, node, "z", mZ);
}

void Point::init(double x, double y, double z)
{
  mNumber = mTotal++;
  mX = x;
  mY = y;
  mZ = z;
}

void Point::move(double dx, double dy, double dz)
{
  mX += dx;
  mY += dy;
  mZ += dz;
}

void Point::setPosition(double x, double y, double z)
{
  mX = x;
  mY = y;
  mZ = z;
}

}
}
}
