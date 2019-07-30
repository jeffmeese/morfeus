#ifndef NODE_H
#define NODE_H

#include "morfeus.h"
#include "morfeusobject.h"

namespace morfeus {
namespace mesh {

class Node
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Node(int32_t number);
  MORFEUS_LIB_DECL Node(int32_t number, double x, double y, double z);

public:
  MORFEUS_LIB_DECL double attribute() const;
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
  MORFEUS_LIB_DECL void setAttribute(double value);
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

private:
  int32_t mNumber;
  bool mBoundary;
  double mAttriubute;
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

}
}

#endif // NODE_H
