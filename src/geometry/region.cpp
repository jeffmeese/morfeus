#include "region.h"

Region::Region()
{
  setPosition(0.0, 0.0);
  setAttribute(0.0);
  setMaxArea(-1.0);
}

Region::Region(double x, double y)
{
  setPosition(x, y);
  setAttribute(0.0);
  setMaxArea(-1.0);
}

double Region::attribute() const
{
  return mAttribute;
}

void Region::doReadFromXml(ptree &tree)
{

}

void Region::doWriteToXml(ptree & parentTree) const
{
  ptree itemTree;

  itemTree.put("<xmlattr>.name", name());
  itemTree.put("<xmlattr>.number", number());
  itemTree.put("<xmlattr>.x", mX);
  itemTree.put("<xmlattr>.y", mY);
  itemTree.put("<xmlattr>.attribute", mAttribute);
  itemTree.put("<xmlattr>.max-area", mMaxArea);

  parentTree.push_back(std::make_pair("Region", itemTree));
}

double Region::maxArea() const
{
  return mMaxArea;
}

double Region::x() const
{
  return mX;
}

double Region::y() const
{
  return mY;
}

void Region::setAttribute(double value)
{
  mAttribute = value;
}

void Region::setMaxArea(double value)
{
  mMaxArea = value;
}

void Region::setPosition(double x, double y)
{
  setX(x);
  setY(y);
}

void Region::setX(double value)
{
  mX = value;
}

void Region::setY(double value)
{
  mY = value;
}

