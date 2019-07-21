#ifndef CIRCLE_H
#define CIRCLE_H

#include "polygon.h"
#include "point2d.h"

class Circle
    : public Polygon
{
public:
  MORFEUS_LIB_DECL Circle();
  MORFEUS_LIB_DECL Circle(const Point2D & center, double radius);
  MORFEUS_LIB_DECL Circle(double x, double y, double radius);

public:
  MORFEUS_LIB_DECL Point2D center() const;
  MORFEUS_LIB_DECL double radius() const;
  MORFEUS_LIB_DECL void setCenter(const Point2D & value);
  MORFEUS_LIB_DECL void setCenter(double x, double y);
  MORFEUS_LIB_DECL void setRadius(double radius);

protected:
  std::vector<Segment> doGetSegmentList() const override;
  std::vector<Vertex> doGetVertexList() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  Point2D mCenter;
  double mRadius;
};

#endif // CIRCLE_H
