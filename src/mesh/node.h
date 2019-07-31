#ifndef MORFEUS_MESH_NODE_H
#define MORFEUS_MESH_NODE_H

#include "morfeus.h"

#include "core/morfeusobject.h"

namespace morfeus {
namespace mesh {

class Node
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL Node(int32_t number);
  MORFEUS_LIB_DECL Node(int32_t number, double x, double y, double z);

public:
  MORFEUS_LIB_DECL bool boundary() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL double scaleX() const;
  MORFEUS_LIB_DECL double scaleY() const;
  MORFEUS_LIB_DECL double scaleZ() const;
  MORFEUS_LIB_DECL void scaling(double & x, double & y, double & z) const;
  MORFEUS_LIB_DECL double translationX() const;
  MORFEUS_LIB_DECL double translationY() const;
  MORFEUS_LIB_DECL double translationZ() const;
  MORFEUS_LIB_DECL void translation(double & x, double & y, double & z) const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL double z() const;
  MORFEUS_LIB_DECL void setBoundary(bool value);
  MORFEUS_LIB_DECL void setPosition(double x, double y, double z);
  MORFEUS_LIB_DECL void setScaling(double x, double y, double z);
  MORFEUS_LIB_DECL void setScaleX(double value);
  MORFEUS_LIB_DECL void setScaleY(double value);
  MORFEUS_LIB_DECL void setScaleZ(double value);
  MORFEUS_LIB_DECL void setTranslation(double x, double y, double z);
  MORFEUS_LIB_DECL void setTranslationX(double value);
  MORFEUS_LIB_DECL void setTranslationY(double value);
  MORFEUS_LIB_DECL void setTranslationZ(double value);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);
  MORFEUS_LIB_DECL void setZ(double value);

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  int32_t mNumber;
  bool mBoundary;
  double mScaleX;
  double mScaleY;
  double mScaleZ;
  double mTransX;
  double mTransY;
  double mTransZ;
  double mX;
  double mY;
  double mZ;
};

inline bool Node::boundary() const
{
  return mBoundary;
}

inline int32_t Node::number() const
{
  return mNumber;
}

inline double Node::scaleX() const
{
  return mScaleX;
}

inline double Node::scaleY() const
{
  return mScaleY;
}

inline double Node::scaleZ() const
{
  return mScaleZ;
}

inline void Node::scaling(double & x, double & y, double & z) const
{
  x = mScaleX;
  y = mScaleY;
  z = mScaleZ;
}

inline double Node::translationX() const
{
  return mTransX;
}

inline double Node::translationY() const
{
  return mTransY;
}

inline double Node::translationZ() const
{
  return mTransZ;
}

inline void Node::translation(double & x, double & y, double & z) const
{
  x = mScaleX;
  y = mScaleY;
  z = mScaleZ;
}

inline double Node::x() const
{
  return mX;
}

inline double Node::y() const
{
  return mY;
}

inline double Node::z() const
{
  return mZ;
}

inline void Node::setBoundary(bool value)
{
  mBoundary = value;
}

inline void Node::setPosition(double x, double y, double z)
{
  mX = x;
  mY = y;
  mZ = z;
}

inline void Node::setScaling(double x, double y, double z)
{
  mScaleX = x;
  mScaleY = y;
  mScaleZ = z;
}

inline void Node::setScaleX(double value)
{
  mScaleX = value;
}

inline void Node::setScaleY(double value)
{
  mScaleY = value;
}

inline void Node::setScaleZ(double value)
{
  mScaleZ = value;
}

inline void Node::setTranslation(double x, double y, double z)
{
  mTransX = x;
  mTransY = y;
  mTransZ = z;
}

inline void Node::setTranslationX(double value)
{
  mTransX = value;
}

inline void Node::setTranslationY(double value)
{
  mScaleY = value;
}

inline void Node::setTranslationZ(double value)
{
  mScaleZ = value;
}

inline void Node::setX(double value)
{
  mX = value;
}

inline void Node::setY(double value)
{
  mY = value;
}

inline void Node::setZ(double value)
{
  mZ = value;
}

}
}

#endif // MORFEUS_MESH_NODE_H
