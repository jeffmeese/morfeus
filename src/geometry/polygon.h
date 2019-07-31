#ifndef MORFEUS_GEOMETRY_POLYGON_H
#define MORFEUS_GEOMETRY_POLYGON_H

#include "morfeus.h"

#include "surface.h"
#include "point.h"

#include <vector>

namespace morfeus {
namespace geometry {

class Polygon
    : public Surface
{
public:
  MORFEUS_LIB_DECL Polygon();
  MORFEUS_LIB_DECL Polygon(const std::string & name);
  MORFEUS_LIB_DECL Polygon(const std::string & id, const std::string & name);

public:
  MORFEUS_LIB_DECL void addPoint(double x, double y, double z);
  MORFEUS_LIB_DECL void addPoint(const Point & point);

protected:
  std::vector<Face*> doGetFaceList() const override;
  std::vector<Segment*> doGetSegmentList() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  std::vector<Point> mPoints;
};

}
}

#endif // MORFEUS_GEOMETRY_POLYGONSHAPE_H
