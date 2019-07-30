#include "application.h"

#include "box.h"
#include "model.h"
#include "mesher.h"
#include "mesh.h"
#include "plane.h"
#include "region.h"

#include <iostream>

int main(int argc, char ** argv)
{
//  std::unique_ptr<Geometry> geometry(new Geometry);

//  // Add the cavity
//  std::unique_ptr<Box> b1(new Box(Point3D(0.0,0.0,-1), 4, 4, 2));
//  geometry->addShape(std::move(b1));

//  std::unique_ptr<Region> r1(new Region("Cavity", Point3D(-1.5, -1.5, -0.5)));
//  r1->setAttribute(4);
//  r1->setMaxArea(0.001);
//  geometry->addRegion(std::move(r1));

//  // Add the patch
//  std::unique_ptr<Box> b2(new Box(Point3D(0.0,0.0,-0.05), 2, 2, 0.1));
//  geometry->addShape(std::move(b2));

//  std::unique_ptr<Region> r2(new Region("Patch", Point3D(0.0, 0.0, -0.005)));
//  r2->setAttribute(1);
//  r2->setMaxArea(0.001);
//  geometry->addRegion(std::move(r2));

//  // Create the mesh
//  std::unique_ptr<Mesh> mesh(new Mesh);
//  Mesher mesher;
//  mesher.createMesh(geometry.get(), mesh.get());

  Application application(argc, argv);
  application.execute();
}
