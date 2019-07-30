#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include "point2d.h"

namespace Morfeus {
namespace Geometry {

class Rectangle
    : public Shape
{
public:
  MORFEUS_LIB_DECL Rectangle();
  MORFEUS_LIB_DECL Rectangle(const std::string & name);
  MORFEUS_LIB_DECL Rectangle(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Rectangle(double xl, double xu, double yl, double yu);
  MORFEUS_LIB_DECL Rectangle(const std::string & name, double xl, double xu, double yl, double yu);
  MORFEUS_LIB_DECL Rectangle(const std::string & id, const std::string & name, double xl, double xu, double yl, double yu);

public:
  MORFEUS_LIB_DECL Point2D lowerLeft() const;
  MORFEUS_LIB_DECL Point2D upperRight() const;
  MORFEUS_LIB_DECL double xl() const;
  MORFEUS_LIB_DECL double xu() const;
  MORFEUS_LIB_DECL double yl() const;
  MORFEUS_LIB_DECL double yu() const;
  MORFEUS_LIB_DECL double z() const;
  MORFEUS_LIB_DECL void setLowerLeft(Point2D value);
  MORFEUS_LIB_DECL void setUpperRight(Point2D value);
  MORFEUS_LIB_DECL void setPosition(double xl, double xu, double yl, double yu);
  MORFEUS_LIB_DECL void setPosition(Point2D lowerLeft, Point2D upperRight);
  MORFEUS_LIB_DECL void setXl(double value);
  MORFEUS_LIB_DECL void setXu(double value);
  MORFEUS_LIB_DECL void setYl(double value);
  MORFEUS_LIB_DECL void setYu(double value);
  MORFEUS_LIB_DECL void setZ(double value);

public:
  MORFEUS_LIB_DECL double height() const;
  MORFEUS_LIB_DECL double width() const;

protected:
  std::vector<Face> doGetFacetList() const override;
  std::vector<Segment> doGetSegmentList() const override;
  std::vector<Vertex> doGetVertexList() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  double mXl;
  double mXu;
  double mYl;
  double mYu;
  double mZ;
};

inline Point2D Rectangle::lowerLeft() const
{
  return Point2D(mXl, mYl);
}

inline Point2D Rectangle::upperRight() const
{
  return Point2D(mXu, mYu);
}

inline double Rectangle::xl() const
{
  return mXl;
}

inline double Rectangle::xu() const
{
  return mXu;
}

inline double Rectangle::yl() const
{
  return mYl;
}

inline double Rectangle::yu() const
{
  return mYu;
}

inline double Rectangle::z() const
{
  return mZ;
}

inline void Rectangle::setLowerLeft(Point2D value)
{
  mXl = value.x();
  mYl = value.y();
}

inline void Rectangle::setUpperRight(Point2D value)
{
  mXu = value.x();
  mYu = value.y();
}

inline void Rectangle::setXl(double value)
{
  mXl = value;
}

inline void Rectangle::setXu(double value)
{
  mXu = value;
}

inline void Rectangle::setYl(double value)
{
  mYl = value;
}

inline void Rectangle::setYu(double value)
{
  mYu = value;
}

inline void Rectangle::setZ(double value)
{
  mZ = value;
}

}
}

#endif // RECTANGLE_H
