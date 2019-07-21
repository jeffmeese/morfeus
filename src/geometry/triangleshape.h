#ifndef TRIANGLESHAPE_H
#define TRIANGLESHAPE_H

#include "polygon.h"

class TriangleShape
    : public Polygon
{
public:
  MORFEUS_LIB_DECL TriangleShape();
  MORFEUS_LIB_DECL TriangleShape(int32_t number);
  MORFEUS_LIB_DECL TriangleShape(const std::string & name, int32_t number);
  MORFEUS_LIB_DECL TriangleShape(double x1, double x2, double x3, double y1, double y2, double y3);
  MORFEUS_LIB_DECL TriangleShape(int32_t number, double x1, double x2, double x3, double y1, double y2, double y3);
  MORFEUS_LIB_DECL TriangleShape(const std::string & name, int32_t number, double x1, double x2, double x3, double y1, double y2, double y3);

public:
  MORFEUS_LIB_DECL double x1() const;
  MORFEUS_LIB_DECL double x2() const;
  MORFEUS_LIB_DECL double x3() const;
  MORFEUS_LIB_DECL double y1() const;
  MORFEUS_LIB_DECL double y2() const;
  MORFEUS_LIB_DECL double y3() const;
  MORFEUS_LIB_DECL void setPosition(double x1, double x2, double x3, double y1, double y2, double y3);
  MORFEUS_LIB_DECL void setX1(double value);
  MORFEUS_LIB_DECL void setX2(double value);
  MORFEUS_LIB_DECL void setX3(double value);
  MORFEUS_LIB_DECL void setY1(double value);
  MORFEUS_LIB_DECL void setY2(double value);
  MORFEUS_LIB_DECL void setY3(double value);

protected:
  std::vector<Segment> doGetSegmentList() const override;
  std::vector<Vertex> doGetVertexList() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void init(double x1, double x2, double x3, double y1, double y2, double y3);

private:
  double mX1;
  double mX2;
  double mX3;
  double mY1;
  double mY2;
  double mY3;
};

inline double TriangleShape::x1() const
{
  return mX1;
}

inline double TriangleShape::x2() const
{
  return mX2;
}

inline double TriangleShape::x3() const
{
  return mX3;
}

inline double TriangleShape::y1() const
{
  return mY1;
}

inline double TriangleShape::y2() const
{
  return mY2;
}

inline double TriangleShape::y3() const
{
  return mY3;
}

inline void TriangleShape::setPosition(double x1, double x2, double x3, double y1, double y2, double y3)
{
  setX1(x1);
  setX2(x2);
  setX3(x3);
  setY1(y1);
  setY2(y2);
  setY3(y3);
}

inline void TriangleShape::setX1(double value)
{
  mX1 = value;
}

inline void TriangleShape::setX2(double value)
{
  mX2 = value;
}

inline void TriangleShape::setX3(double value)
{
  mX3 = value;
}

inline void TriangleShape::setY1(double value)
{
  mY1 = value;
}

inline void TriangleShape::setY2(double value)
{
  mY2 = value;
}

inline void TriangleShape::setY3(double value)
{
  mY3 = value;
}

#endif // TRIANGLESHAPE_H
