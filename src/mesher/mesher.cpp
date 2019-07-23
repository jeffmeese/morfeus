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
#include "xmlutils.h"

#include <iostream>

extern "C"
{
  #include "triangulate.h"
}

static const std::string OBJECT_ID("Mesher");

Mesher::Mesher()
  : MorfeusObject(OBJECT_ID)
  , mMaxEdgeLength(1.0)
  , mMaxTriangleArea(-1.0)
  , mMinEdgeLength(-1.0)
  , mMinTriangleArea(-1.0)
{
}

Mesher::~Mesher()
{
}

void Mesher::createMesh(const Geometry * geometry, double cavityHeight, Mesh * mesh) const
{
  std::unique_ptr<Mesh> surfaceMesh(new Mesh);

  createSurfaceMesh(geometry, mesh);
  //extrudeVolumeMesh(surfaceMesh.get(), cavityHeight, mesh);
}

void Mesher::createSurfaceMesh(const Geometry *geometry, Mesh *mesh) const
{
  triangulateio in, out;

  in.numberofpoints = 0;
  in.numberofpointattributes = 1;
  in.numberofsegments = 0;
  for (std::size_t i = 0; i < geometry->totalShapes(); i++) {
    const Shape * shape = geometry->shape(i);
    std::vector<Vertex> vertexList = shape->getVertexList();
    std::vector<Segment> segmentList = shape->getSegmentList();
    in.numberofpoints += static_cast<int>(vertexList.size());
    in.numberofsegments += static_cast<int>(segmentList.size());
  }

  in.numberofholes = static_cast<int>(geometry->totalHoles());
  in.numberofregions = static_cast<int>(geometry->totalRegions());

  uint64_t numPoints = static_cast<uint64_t>(in.numberofpoints);
  uint64_t numPointAttrs = static_cast<uint64_t>(in.numberofpointattributes);
  uint64_t numSeqments = static_cast<uint64_t>(in.numberofsegments);
  uint64_t numHoles = static_cast<uint64_t>(in.numberofholes);
  uint64_t numRegions = static_cast<uint64_t>(in.numberofregions);

  if (numPoints > 0) {
    in.pointlist = new double[static_cast<uint64_t>(numPoints*2)];
    in.pointattributelist = new double[numPoints*numPointAttrs];
    in.pointmarkerlist = new int[numPoints];
  }

  if (numSeqments > 0) {
    in.segmentlist = new int[numSeqments*2];
    in.segmentmarkerlist = new int[numSeqments];
  }

  if (numRegions > 0) {
    in.regionlist = new double[numRegions*4];
  }

  if (numHoles > 0) {
    in.holelist = new double[numHoles*2];
  }

  std::size_t regionIndex = 0;
  for (std::size_t i = 0; i < geometry->totalRegions(); i++) {
    const Region * region = geometry->region(i);
    in.regionlist[regionIndex] = region->x();
    in.regionlist[regionIndex+1] = region->y();
    in.regionlist[regionIndex+2] = region->attribute();
    in.regionlist[regionIndex+3] = region->maxArea();
    regionIndex += 4;
  }

  std::size_t pointIndex = 0;
  std::size_t segmentIndex = 0;
  std::size_t pointAttrIndex = 0;
  std::size_t segmentMarkerIndex = 0;
  int32_t vertexOffset = 0;
  for (std::size_t i = 0; i < geometry->totalShapes(); i++) {
    const Shape * shape = geometry->shape(i);
    std::vector<Vertex> vertexList = shape->getVertexList();
    std::vector<Segment> segmentList = shape->getSegmentList();

    for (std::size_t j = 0; j < vertexList.size(); j++) {
      const Vertex & v = vertexList.at(j);
      in.pointlist[pointIndex] = v.x();
      in.pointlist[pointIndex+1] = v.y();
      in.pointmarkerlist[pointAttrIndex] = v.boundary();
      in.pointattributelist[pointAttrIndex] = 0.0;
      pointIndex += 2;
      pointAttrIndex++;
    }

    for (std::size_t j = 0; j < segmentList.size(); j++) {
      const Segment & s = segmentList.at(j);
      in.segmentlist[segmentIndex] = s.node1() + vertexOffset;
      in.segmentlist[segmentIndex+1] = s.node2() + vertexOffset;
      in.segmentmarkerlist[segmentMarkerIndex] = s.boundary();
      segmentIndex += 2;
      segmentMarkerIndex++;
    }

    vertexOffset += static_cast<int32_t>(vertexList.size());
  }

  std::size_t holeIndex = 0;
  for (std::size_t i = 0; i < geometry->totalHoles(); i++) {
    const Hole * hole = geometry->hole(i);
    in.holelist[holeIndex] = hole->x();
    in.holelist[holeIndex+1] = hole->y();
    holeIndex += 2;
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

  std::size_t nodeIndex = 0;
  for (int32_t i = 0; i < out.numberofpoints; i++) {
    double x = out.pointlist[nodeIndex];
    double y = out.pointlist[nodeIndex+1];
    double attribute = out.pointattributelist[i];
    int32_t boundary = out.pointmarkerlist[i];
    nodeIndex += 2;

    std::unique_ptr<Node> node(new Node(i, x, y, 0.0));
    node->setBoundary(boundary);
    node->setAttribute(attribute);
    mesh->addNode(std::move(node));
  }

  std::size_t triIndex = 0;
  int32_t attrIndex = 0;
  for (int32_t i = 0; i < out.numberoftriangles; i++) {
    int32_t node1 = out.trianglelist[triIndex] + 1;
    int32_t node2 = out.trianglelist[triIndex+1] + 1;
    int32_t node3 = out.trianglelist[triIndex+2] + 1;

    std::unique_ptr<TriangleElement> triangle(new TriangleElement(i));
    triangle->setNode(0, node1);
    triangle->setNode(1, node2);
    triangle->setNode(2, node3);

    for (int32_t j = 0; j < out.numberoftriangleattributes; j++) {
      double attribute = out.triangleattributelist[attrIndex];
      attrIndex += 1;
      triangle->addAttribute(attribute);
    }

    mesh->addElement(std::move(triangle));
    triIndex += 3;
  }

  if (numHoles > 0)
    delete [] in.holelist;

  if (numRegions > 0)
    delete [] in.regionlist;

  if (numPoints > 0) {
    delete [] in.pointlist;
    delete [] in.pointmarkerlist;
    delete [] in.pointattributelist;
  }

  if (numSeqments > 0) {
    delete [] in.segmentlist;
    delete [] in.segmentmarkerlist;
  }

  if (out.edgelist != nullptr) free(out.edgelist);
  if (out.edgemarkerlist != nullptr) free(out.edgemarkerlist);
  if (out.pointlist != nullptr) free(out.pointlist);
  if (out.pointattributelist != nullptr) free(out.pointattributelist);
  if (out.pointmarkerlist != nullptr) free(out.pointmarkerlist);
  if (out.segmentlist != nullptr) free(out.segmentlist);
  if (out.segmentmarkerlist != nullptr) free(out.segmentmarkerlist);
  if (out.trianglelist != nullptr) free(out.trianglelist);
  if (out.trianglearealist != nullptr) free(out.trianglearealist);
  if (out.triangleattributelist != nullptr) free(out.triangleattributelist);
}

void Mesher::print(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Mesher");
  xmlutils::printValue(output, tabPos+2, "Min Edge Length: ", mMinEdgeLength);
  xmlutils::printValue(output, tabPos+2, "Max Edge Length: ", mMaxEdgeLength);
  xmlutils::printValue(output, tabPos+2, "Min Triangle Area: ", mMinTriangleArea);
  xmlutils::printValue(output, tabPos+2, "Max Triangle Area: ", mMaxTriangleArea);
  xmlutils::printHeader(output, tabPos, "End Mesher");
  output << "\n";
}

void Mesher::print(int tabPos) const
{
  print(std::cout, tabPos);
}

void Mesher::readFromXml(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setMinEdgeLength(xmlutils::readAttribute<double>(node, "min-edge-length"));
  setMaxEdgeLength(xmlutils::readAttribute<double>(node, "max-edge-length"));
  setMinTriangleArea(xmlutils::readAttribute<double>(node, "min-triangle-area"));
  setMaxTriangleArea(xmlutils::readAttribute<double>(node, "max-triangle-area"));
}

void Mesher::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * mesherNode = xmlutils::createNode(document, "Mesher");
  xmlutils::writeAttribute(document, mesherNode, "min-edge-length", mMinEdgeLength);
  xmlutils::writeAttribute(document, mesherNode, "max-edge-length", mMaxEdgeLength);
  xmlutils::writeAttribute(document, mesherNode, "min-triangle-area", mMinTriangleArea);
  xmlutils::writeAttribute(document, mesherNode, "max-triangle-area", mMaxTriangleArea);
  node->append_node(mesherNode);
}

void Mesher::extrudeVolumeMesh(const Mesh *surfaceMesh, double cavityHeight, const Mesh *volumeMesh) const
{

}
