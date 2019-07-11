#include "mesh.h"
#include "mesher.h"
#include "geometry.h"
#include "region.h"
#include "segment.h"
#include "vertex.h"

#include <iostream>

int main()
{
  Geometry geometry;

  std::unique_ptr<Vertex> v1(new Vertex(0, -1.0, -1.0));
  std::unique_ptr<Vertex> v2(new Vertex(1, 1.0, -1.0));
  std::unique_ptr<Vertex> v3(new Vertex(2, 1.0, 1.0));
  std::unique_ptr<Vertex> v4(new Vertex(3, -1.0, 1.0));

  geometry.addVertex(std::move(v1));
  geometry.addVertex(std::move(v2));
  geometry.addVertex(std::move(v3));
  geometry.addVertex(std::move(v4));

  std::unique_ptr<Segment> s1(new Segment(0, 0, 1));
  std::unique_ptr<Segment> s2(new Segment(1, 1, 2));
  std::unique_ptr<Segment> s3(new Segment(2, 2, 3));
  std::unique_ptr<Segment> s4(new Segment(3, 3, 0));

  geometry.addSegment(std::move(s1));
  geometry.addSegment(std::move(s2));
  geometry.addSegment(std::move(s3));
  geometry.addSegment(std::move(s4));

  std::unique_ptr<Region> region(new Region(0));
  region->setPosition(0.0, 0.0);
  region->setMaxArea(0.01);
  geometry.addRegion(std::move(region));

  Mesh mesh;
  Mesher mesher;
  mesher.createMesh(geometry, mesh);

  mesh.writeGeomFile("c:\\users\\jmeese\\desktop\\t.geom");
}
