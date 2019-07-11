#include "region.h"

Region::Region(int32_t id)
  : mId(id)
{
  setPosition(0.0, 0.0);
  setAttribute(0.0);
  setMaxArea(-1.0);
}

Region::Region(int32_t id, double x, double y)
  : mId(id)
{
  setPosition(x, y);
  setAttribute(0.0);
  setMaxArea(-1.0);
}

double Region::attribute() const
{
  return mAttribute;
}

int32_t Region::id() const
{
  return mId;
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

