#ifndef POLYGON_H
#define POLYGON_H

#include "morfeus.h"
#include "shape.h"
#include "point2d.h"

#include <vector>

class Polygon
    : public Shape
{
public:
  MORFEUS_LIB_DECL Polygon();
  MORFEUS_LIB_DECL Polygon(const std::string & name);
  MORFEUS_LIB_DECL Polygon(const std::string & id, const std::string & name);

public:
  MORFEUS_LIB_DECL void addPoint(double x, double y);
  MORFEUS_LIB_DECL void addPoint(const Point2D & point);

protected:
  std::vector<Segment> doGetSegmentList() const override;
  std::vector<Vertex> doGetVertexList() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  std::vector<Point2D> mPoints;
};

#endif // POLYGON_H
