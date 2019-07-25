#include "mesherpolygon.h"

#include <cassert>

MesherPolygon::MesherPolygon()
{

}

void MesherPolygon::addPoint(double x, double y, double z)
{
  addPoint(Point3D(x, y, z));
}

void MesherPolygon::addPoint(const Point3D &point)
{
  mPoints.push_back(point);
}

void MesherPolygon::computeNormal(double * v) const
{
  assert(totalPoints() >= 3);

  double v1[3], v2[3], v3[3], v4[3], v5[3];

  for (std::size_t i = 0; i < 3; i++) {
    const Point3D & pt = point(i);
    v1[i] = pt.x();
    v2[i] = pt.y();
    v3[i] = pt.z();
  }

  for (std::size_t i = 0; i < 3; i++) {
    v4[i] = v1[i] - v2[i];
    v5[i] = v1[i] - v3[i];
  }

  v[0] = v4[1]*v5[2] - v4[2]*v5[1];
  v[1] = v4[2]*v5[0] - v4[0]*v5[2];
  v[2] = v4[0]*v5[1] - v4[1]*v5[0];

  //boost::numeric::ublas::vector<double> v4(v1-v2);
  //boost::numeric::ublas::vector<double> v5(v1-v3);
  //return boost::geometry::cross_product(v4, v5);
}

bool MesherPolygon::isCoplanar(const MesherPolygon & polygon) const
{
  double tolerance = 1e-8;

  double n1[3], n2[3];
  computeNormal(n1);
  polygon.computeNormal(n2);

  double dx = fabs(n1[0]-n2[0]);
  double dy = fabs(n1[1]-n2[1]);
  double dz = fabs(n1[2]-n2[2]);

  return ( (dx + dy + dx) < tolerance);
}
