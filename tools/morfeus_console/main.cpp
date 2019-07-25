#include "application.h"

#include "mesherpolygon.h"

#include <iostream>

int main(int argc, char ** argv)
{
  MesherPolygon p1;
  MesherPolygon p2;

  p1.addPoint(Point3D(-2.0, -2.0, 0.0));
  p1.addPoint(Point3D(2.0, 2.0, 0.0));
  p1.addPoint(Point3D(2.0, 2.0, 0.0));
  p1.addPoint(Point3D(-2.0, 2.0, 0.0));

  p2.addPoint(Point3D(-1.0, -1.0, -1.0));
  p2.addPoint(Point3D(1.0, 1.0, -1.0));
  p2.addPoint(Point3D(1.0, 1.0, -1.0));
  p2.addPoint(Point3D(-1.0, 1.0, -1.0));

  std::cout << p1.isCoplanar(p2) << "\n";

  //Application application(argc, argv);
  //application.execute();
}
