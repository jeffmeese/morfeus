#include "mesher.h"

#include "edge.h"
#include "geometry.h"
#include "hole.h"
#include "mesh.h"
#include "meshrefinement.h"
#include "node.h"
#include "region.h"
#include "segment.h"
#include "triangleelement.h"
#include "vertex.h"

#include <iostream>

extern "C"
{
  #include "triangulate.h"
}

Mesher::Mesher()
{
}

Mesher::~Mesher()
{
}

void Mesher::createMesh(const Geometry & geometry, Mesh & mesh)
{
  triangulateio in, out;
  int index = 0;

  in.numberofpoints = geometry.totalVertices();
  in.numberofpointattributes = 1;
  in.numberofregions = geometry.totalRegions();
  in.numberofsegments = geometry.totalSegments();
  in.numberofholes = geometry.totalHoles();

  uint64_t numPoints = static_cast<uint64_t>(in.numberofpoints);
  uint64_t numPointAttrs = static_cast<uint64_t>(in.numberofpointattributes);
  uint64_t numSeqments = static_cast<uint64_t>(in.numberofsegments);
  uint64_t numHoles = static_cast<uint64_t>(in.numberofholes);
  uint64_t numRegions = static_cast<uint64_t>(in.numberofregions);

  in.pointlist = new double[static_cast<uint64_t>(numPoints*2)];
  in.pointattributelist = new double[numPoints*numPointAttrs];
  in.pointmarkerlist = new int[numPoints];
  in.segmentlist = new int[numSeqments*2];
  in.segmentmarkerlist = new int[numSeqments];
  in.regionlist = new double[numRegions*4];
  in.holelist = new double[numHoles*2];

  for (int32_t i = 0; i < in.numberofpoints; i++) {
    const Vertex * v = geometry.vertex(i);
    in.pointlist[index] = v->x();
    in.pointlist[index+1] = v->y();
    in.pointattributelist[i] = 0.0;
    in.pointmarkerlist[i] = 0;
    index += 2;
  }

  index = 0;
  for (int32_t i = 0; i < in.numberofsegments; i++) {
    const Segment * segment = geometry.segment(i);
    in.segmentlist[index] = segment->node1();
    in.segmentlist[index+1] = segment->node2();
    in.segmentmarkerlist[i] = segment->boundary();
    index += 2;
  }

  index = 0;
  for (int32_t i = 0; i < in.numberofholes; i++) {
    const Hole * hole = geometry.hole(i);
    in.holelist[index] = hole->x();
    in.holelist[index+1] = hole->y();
    index += 2;
  }

  index = 0;
  for (int32_t i = 0; i < in.numberofregions; i++) {
    const Region * region = geometry.region(i);
    in.regionlist[index] = region->x();
    in.regionlist[index+1] = region->y();
    in.regionlist[index+2] = region->attribute();
    in.regionlist[index+3] = region->maxArea();
    index += 4;
  }

  out.edgelist = nullptr;
  out.edgemarkerlist = nullptr;
  out.pointlist = nullptr;
  out.pointattributelist = nullptr;
  out.pointmarkerlist = nullptr;
  out.segmentlist = nullptr;
  out.segmentmarkerlist = nullptr;
  out.trianglelist = nullptr;
  out.trianglearealist = nullptr;
  out.triangleattributelist = nullptr;

  char command[] = "pcqazeAQ";
  triangulate(command, &in, &out, nullptr);

  index = 0;
  for (int32_t i = 0; i < out.numberofpoints; i++) {
    double x = out.pointlist[index];
    double y = out.pointlist[index+1];
    double attribute = out.pointattributelist[i];
    int32_t boundary = out.pointmarkerlist[i];
    index += 2;

    std::unique_ptr<Node> node(new Node(i+1, x, y, 0.0));
    node->setBoundary(boundary);
    node->setAttribute(attribute);

    mesh.addNode(std::move(node));
  }

  index = 0;
  int32_t attrIndex = 0;
  for (int32_t i = 0; i < out.numberoftriangles; i++) {
    int32_t node1 = out.trianglelist[index] + 1;
    int32_t node2 = out.trianglelist[index+1] + 1;
    int32_t node3 = out.trianglelist[index+2] + 1;

    std::unique_ptr<TriangleElement> triangle(new TriangleElement(i+1));
    triangle->setNode(1, node1);
    triangle->setNode(2, node2);
    triangle->setNode(3, node3);

    for (int32_t j = 0; j < out.numberoftriangleattributes; j++) {
      double attribute = out.triangleattributelist[attrIndex];
      attrIndex += 1;
      triangle->addAttribute(attribute);
    }

    mesh.addElement(std::move(triangle));
    index += 3;
  }

  delete [] in.holelist;
  delete [] in.regionlist;
  delete [] in.pointlist;
  delete [] in.pointmarkerlist;
  delete [] in.pointattributelist;
  delete [] in.segmentlist;
  delete [] in.segmentmarkerlist;

  if (out.edgelist != nullptr) free(out.edgelist);
  if (out.edgemarkerlist != nullptr) free(out.edgemarkerlist);
  //if (out.neighborlist != nullptr) free(out.neighborlist);
  if (out.pointlist != nullptr) free(out.pointlist);
  if (out.pointattributelist != nullptr) free(out.pointattributelist);
  if (out.pointmarkerlist != nullptr) free(out.pointmarkerlist);
  if (out.segmentlist != nullptr) free(out.segmentlist);
  if (out.segmentmarkerlist != nullptr) free(out.segmentmarkerlist);
  if (out.trianglelist != nullptr) free(out.trianglelist);
  if (out.trianglearealist != nullptr) free(out.trianglearealist);
  if (out.triangleattributelist != nullptr) free(out.triangleattributelist);
}

void Mesher::refineMesh(const Mesh &oldMesh, const MeshRefinement &refinement, Mesh &mesh)
{

}
