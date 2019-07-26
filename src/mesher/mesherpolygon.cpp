#include "mesherpolygon.h"

#include <cassert>
#include <iostream>

#include <boost/qvm/vec_operations.hpp>

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

void MesherPolygon::clearPoints()
{
  mPoints.clear();
}

boost::qvm::vec<double, 3> MesherPolygon::computeNormal() const
{
  assert(totalPoints() >= 3);

  double v[3];
  double v1[3], v2[3], v3[3], v4[3], v5[3];

  v1[0] = mPoints[0].x();
  v1[1] = mPoints[0].y();
  v1[2] = mPoints[0].z();
  v2[0] = mPoints[1].x();
  v2[1] = mPoints[1].y();
  v2[2] = mPoints[1].z();
  v3[0] = mPoints[2].x();
  v3[1] = mPoints[2].y();
  v3[2] = mPoints[2].z();

  for (std::size_t i = 0; i < 3; i++) {
    v4[i] = v1[i] - v2[i];
    v5[i] = v1[i] - v3[i];
  }

  v[0] = v4[1]*v5[2] - v4[2]*v5[1];
  v[1] = v4[2]*v5[0] - v4[0]*v5[2];
  v[2] = v4[0]*v5[1] - v4[1]*v5[0];

  boost::qvm::vec<double, 3> n = {v[0], v[1], v[2]};
  return n;
}

//! Determines if two polygons are coplanar
//!
//! This function will determine if the current polygon
//! is coplanar to the supplied polygon. First the normal
//! vector to this polygon is determined using the first
//! three points, (P1, P2, P3) using the formula:
//! P1P2 x P1P3
//!
//! Next, each point in the supplied polygon is checked to
//! see if it is coplanar to the polygon plane. To be coplanar
//! the vector from P1 to any point on the supplied polygon (Pn)
//! must be othorgonal to the normal vector of this polygon
//! (i.e. P1Pn . (P1P2 x P1P3) == 0. If all points on the supplied
//! polygon are coplanar within the supplied tolerance
//! the supplied polygon is deemed to be coplanar to this polygon
//!
//! NOTE: This assumes the current polygon has a least three points
//!
bool MesherPolygon::isCoplanar(const MesherPolygon & polygon, double tolerance) const
{
  // First determine the normal vector using the first three points of this polygon
  boost::qvm::vec<double, 3> nv = computeNormal();

  //boost::qvm::vec<double, 3> nv = {n[0], n[1], n[2]} ;
  //nv = {n[0], n[1], n[2]};

  // Next determine if each point in the supplied polygon is coplanar with this polygon.
  for (std::size_t i = 0; i < polygon.totalPoints(); i++) {
    const Point3D & pt = polygon.point(i);

    // Compute the vector from the first point of this polygon to the current point
    boost::qvm::vec<double, 3> v = {pt.x(), pt.y(), pt.z()} ;

    // Compute the dot product of the vector v to the normal vector n
    double dot = boost::qvm::dot(nv, v);
    if (fabs(dot) >= tolerance)
      return false;
  }

  return true;
}
