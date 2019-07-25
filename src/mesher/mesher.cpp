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
#include <set>

#include "tetgen.h"

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

//! Creates the volume mesh from the geometry
//!
//! This function creates the volume mesh from the geometry specification
//! using the supplied cavity height. The mesh generation is done using the
//! tetgen library which requires the geometry be formatted in a particular way.
//! Tetgen requires a list of nodes, a list of facets (composed of one or
//! more polygons and holes), a list of non-polygon related holes, and
//! a list of regions that may contain attributes and/or area constraints.
//!
//! The general algorithm is as follows:
//! 1. Identify all the polygonal shapes on the surface of the geometry
//! 2. Identify multiple shapes in the same plane that must belong to the same facet in tetgen.
//! 3. Create additional nodes to generate the 3-D domain to be meshed by tetgen
//! 4. Populate the tetgenio input data structure with the nodes and facets
//! 5. Populate the tetgenio input data structure with any other holes and regions
//! 6. Call tetrahedralize to create the volume mesh
//! 7. Copy the relevant data from the tetgenio output data structure to the appropriate mesh format
//!
//! NOTE: Currently only 2-D shape specifications are allowed in the program so all
//! shapes in the geometry should be in the same plane. This may change in the future
//! if the program allows more complicated feed structures such as microstrip feeds
//! or coax feeds in which shapes will need to be specified that are not on the surface
//! of the geometry. The routine should hopefully be general enough to handle these
//! situations.
//!
void Mesher::createMesh(const Geometry * geometry, double cavityHeight, Mesh * mesh) const
{
  tetgenio in;
  tetgenio out;

  // Set the number of facets, holes, and regions
  in.numberoffacets = static_cast<int>(geometry->totalShapes());
  in.numberofholes = static_cast<int>(geometry->totalHoles());
  in.numberofregions = static_cast<int>(geometry->totalRegions());

  // Determine the number of points for all shapes
  in.numberofpoints = 0;
  in.numberofpointattributes = 0;
  for (std::size_t i = 0; i < geometry->totalShapes(); i++) {
    const Shape * shape = geometry->shape(i);
    std::vector<MesherPolygon> polygons = shape->getMesherPolygons();
    for (std::size_t j = 0; j < polygons.size(); j++) {
      in.numberofpoints += static_cast<int>(polygons.at(i).totalPoints());
    }
  }

  // Allocate points
  if (in.numberofpoints > 0) {
    in.pointlist = new double[static_cast<uint64_t>(in.numberofpoints*2)];
    in.pointmarkerlist = new int[in.numberofpoints];
  }

  // Allocate regions
  if (in.numberofregions > 0) {
    in.regionlist = new double[in.numberofregions*4];
  }

  // Allocate holes
  if (in.numberofholes > 0) {
    in.holelist = new double[in.numberofholes*2];
  }

  // Allocate facets
  if (in.numberoffacets > 0) {
    in.facetlist = new tetgenio::facet[in.numberoffacets];
    in.facetmarkerlist = new int[in.numberoffacets];
  }

  // Write points to input structure
  std::size_t pointIndex = 0;
  std::size_t pointMarkerIndex = 0;
  int32_t vertexOffset = 0;
  for (std::size_t i = 0; i < geometry->totalShapes(); i++) {
    const Shape * shape = geometry->shape(i);
    std::vector<MesherPolygon> polygons = shape->getMesherPolygons();

    for (std::size_t j = 0; j < polygons.size(); j++) {
      const MesherPolygon & p = polygons.at(j);
      for (std::size_t k = 0; k < p.totalPoints(); k++) {
        const Point3D & pt = p.point(k);
        in.pointlist[pointIndex] = pt.x();
        in.pointlist[pointIndex+1] = pt.y();
        in.pointlist[pointIndex+2] = pt.z();
        in.pointmarkerlist[pointMarkerIndex] = 0;
        pointIndex += 3;
        pointMarkerIndex++;
      }
    }
  }

//    in.facetmarkerlist[i] = 0;
//    tetgenio::facet * f = &in.facetlist[i];
//    f->numberofpolygons = 1;
//    f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
//    f->numberofholes = 0;
//    f->holelist = nullptr;
//    tetgenio::polygon * p = &f->polygonlist[0];
//    p->numberofvertices = vertexList.size();

//    for (std::size_t j = 0; j < vertexList.size(); j++) {
//      p->vertexlist[j] = vertexOffset + j;
//    }
//    vertexOffset += vertexList.size();
//  }

  // Write facets to input structure
  //
  // Each facet is composed of one or more polygons and one or more holes
  // Currently, shapes don't support holes so we only need to specify
  // polygons. Each shape in the geometry is composed of one or more
  // polygons as well. If two or more polygons from multiple shapes are
  // co-planar we must specify each of those polygons in the same facet
  // so that tetgen can properly understand the geometry.
  std::set<std::size_t> shapesDone;
  for (std::size_t i = 0; i < geometry->totalShapes(); i++) {
    const Shape * shape1 = geometry->shape(i);
    std::vector<MesherPolygon> polygons = shape1->getMesherPolygons();

    tetgenio::facet * f = &in.facetlist[i];
    f->numberofpolygons = 1;
    for (std::size_t j = 0; j < geometry->totalShapes(); j++) {
      const Shape * shape2 = geometry->shape(j);

      // Ignore comparing a shape to itself
      if (i != j) {

      }
    }
  }


  // Write regions to input structure
  std::size_t regionIndex = 0;
  for (std::size_t i = 0; i < geometry->totalRegions(); i++) {
    const Region * region = geometry->region(i);
    in.regionlist[regionIndex] = region->x();
    in.regionlist[regionIndex+1] = region->y();
    in.regionlist[regionIndex+2] = region->z();
    in.regionlist[regionIndex+3] = region->attribute();
    in.regionlist[regionIndex+4] = region->maxArea();
    regionIndex += 5;
  }

  // Write holes to input structure
  std::size_t holeIndex = 0;
  for (std::size_t i = 0; i < geometry->totalHoles(); i++) {
    const Hole * hole = geometry->hole(i);
    in.holelist[holeIndex] = hole->x();
    in.holelist[holeIndex+1] = hole->y();
    in.holelist[holeIndex+2] = hole->z();
    holeIndex += 3;
  }

//  char switches[] = "pqaA";
//  tetrahedralize(switches, &in, &out);

//  std::cout << out.numberofpoints << "\n";
//  std::cout << out.numberoftetrahedra << "\n";

  //std::unique_ptr<Mesh> surfaceMesh(new Mesh);
  //createSurfaceMesh(geometry, mesh);
  //extrudeVolumeMesh(surfaceMesh.get(), cavityHeight, mesh);
}

void Mesher::formFacets(std::vector<MesherFacet> &facets, const Geometry &geometry)
{
  std::vector<MesherPolygon> allPolygons;
  for (std::size_t i = 0; i < geometry.totalShapes(); i++) {
    const Shape * shape = geometry.shape(i);
    std::vector<MesherPolygon> polygons = shape->getMesherPolygons();
    for (std::size_t j = 0; j < polygons.size(); j++) {
      allPolygons.push_back(polygons.at(j));
    }
  }
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

