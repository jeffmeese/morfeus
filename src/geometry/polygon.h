#ifndef POLYGON_H
#define POLYGON_H

#include "morfeus.h"
#include "shape.h"
#include "point3d.h"

#include <vector>

namespace Morfeus {
namespace Geometry {

class Polygon
    : public Shape
{
public:
  MORFEUS_LIB_DECL Polygon();
  MORFEUS_LIB_DECL Polygon(const std::string & name);
  MORFEUS_LIB_DECL Polygon(const std::string & id, const std::string & name);

public:
  MORFEUS_LIB_DECL void addPoint(double x, double y, double z);
  MORFEUS_LIB_DECL void addPoint(const Point3D & point);

protected:
  std::vector<Face> doGetFacetList() const override;
  std::vector<Segment> doGetSegmentList() const override;
  std::vector<Vertex> doGetVertexList() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  std::vector<Point3D> mPoints;
};

}
}

#endif // POLYGONSHAPE_H
