#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "polygon.h"
#include "point2d.h"

class Rectangle
    : public Polygon
{
public:
  MORFEUS_LIB_DECL Rectangle();

public:
  MORFEUS_LIB_DECL double bottom() const;
  MORFEUS_LIB_DECL double left() const;
  MORFEUS_LIB_DECL double right() const;
  MORFEUS_LIB_DECL double top() const;
  MORFEUS_LIB_DECL void setBottom(double value);
  MORFEUS_LIB_DECL void setLeft(double value);
  MORFEUS_LIB_DECL void setPosition(double left, double right, double top, double bottom);
  MORFEUS_LIB_DECL void setRight(double value);
  MORFEUS_LIB_DECL void setTop(double value);

public:
  MORFEUS_LIB_DECL double height() const;
  MORFEUS_LIB_DECL double width() const;

protected:
  void doAddToGeometry(Geometry * geometry) const override;
  void doReadFromXml(ptree & tree) override;
  void doWriteToXml(ptree & tree) const override;

private:
  double mBottom;
  double mLeft;
  double mRight;
  double mTop;
};

inline double Rectangle::bottom() const
{
  return mBottom;
}

inline double Rectangle::left() const
{
  return mLeft;
}

inline double Rectangle::right() const
{
  return mRight;
}

inline double Rectangle::top() const
{
  return mTop;
}

inline void Rectangle::setBottom(double value)
{
  mBottom = value;
}

inline void Rectangle::setLeft(double value)
{
  mLeft = value;
}

inline void Rectangle::setRight(double value)
{
  mRight = value;
}

inline void Rectangle::setTop(double value)
{
  mTop = value;
}

#endif // RECTANGLE_H
