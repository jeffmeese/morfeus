#include "node.h"

Node::Node(int32_t number)
  : MorfeusObject("Node")
  , mNumber(number)
{
  setPosition(0.0, 0.0, 0.0);
  setBoundary(false);
  setAttribute(0.0);
  setScaling(1.0, 1.0, 1.0);
  setTranslation(0.0, 0.0, 0.0);
}

Node::Node(int32_t number, double x, double y, double z)
  : MorfeusObject("Node")
  , mNumber(number)
{
  setPosition(x, y, z);
  setBoundary(false);
  setAttribute(0.0);
  setScaling(1.0, 1.0, 1.0);
  setTranslation(0.0, 0.0, 0.0);
}

double Node::attribute() const
{
  return mAttriubute;
}

bool Node::boundary() const
{
  return mBoundary;
}

int32_t Node::number() const
{
  return mNumber;
}

double Node::scaleX() const
{
  return mScaleX;
}

double Node::scaleY() const
{
  return mScaleY;
}

double Node::scaleZ() const
{
  return mScaleZ;
}

void Node::scaling(double & x, double & y, double & z) const
{
  x = mScaleX;
  y = mScaleY;
  z = mScaleZ;
}

double Node::translationX() const
{
  return mTransX;
}

double Node::translationY() const
{
  return mTransY;
}

double Node::translationZ() const
{
  return mTransZ;
}

void Node::translation(double & x, double & y, double & z) const
{
  x = mScaleX;
  y = mScaleY;
  z = mScaleZ;
}

double Node::x() const
{
  return mX;
}

double Node::y() const
{
  return mY;
}

double Node::z() const
{
  return mZ;
}

void Node::setAttribute(double value)
{
  mAttriubute = value;
}

void Node::setBoundary(bool value)
{
  mBoundary = value;
}

void Node::setPosition(double x, double y, double z)
{
  mX = x;
  mY = y;
  mZ = z;
}

void Node::setScaling(double x, double y, double z)
{
  mScaleX = x;
  mScaleY = y;
  mScaleZ = z;
}

void Node::setScaleX(double value)
{
  mScaleX = value;
}

void Node::setScaleY(double value)
{
  mScaleY = value;
}

void Node::setScaleZ(double value)
{
  mScaleZ = value;
}

void Node::setTranslation(double x, double y, double z)
{
  mTransX = x;
  mTransY = y;
  mTransZ = z;
}

void Node::setTranslationX(double value)
{
  mTransX = value;
}

void Node::setTranslationY(double value)
{
  mScaleY = value;
}

void Node::setTranslationZ(double value)
{
  mScaleZ = value;
}

void Node::setX(double value)
{
  mX = value;
}

void Node::setY(double value)
{
  mY = value;
}

void Node::setZ(double value)
{
  mZ = value;
}
