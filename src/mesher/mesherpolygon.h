#ifndef MESHERPOLYGON_H
#define MESHERPOLYGON_H

#include "morfeus.h"
#include "point3d.h"

#include <vector>

class MesherPolygon
{
public:
  MORFEUS_LIB_DECL MesherPolygon();

public:
  MORFEUS_LIB_DECL const Point3D & point(std::size_t index) const;
  MORFEUS_LIB_DECL std::size_t totalPoints() const;

public:
  MORFEUS_LIB_DECL void addPoint(double x, double y, double z);
  MORFEUS_LIB_DECL void addPoint(const Point3D & point);
  MORFEUS_LIB_DECL void computeNormal(double * v) const;
  MORFEUS_LIB_DECL bool isCoplanar(const MesherPolygon & polygon) const;

private:
  std::vector<Point3D> mPoints;
};

inline const Point3D & MesherPolygon::point(std::size_t index) const
{
  return mPoints.at(index);
}

inline std::size_t MesherPolygon::totalPoints() const
{
  return mPoints.size();
}

#endif // MESHERPOLYGON_H
