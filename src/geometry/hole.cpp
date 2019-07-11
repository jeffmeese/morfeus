#include "hole.h"

Hole::Hole(int32_t id)
  : mId(id)
{
  setPosition(0.0, 0.0);
}

Hole::Hole(int32_t id, double x, double y)
  : mId(id)
{
  setPosition(x, y);
}

int32_t Hole::id() const
{
  return mId;
}

double Hole::x() const
{
  return mX;
}

double Hole::y() const
{
  return mY;
}

void Hole::setPosition(double x, double y)
{
  setX(x);
  setY(y);
}

void Hole::setX(double value)
{
  mX = value;
}

void Hole::setY(double value)
{
  mY = value;
}
