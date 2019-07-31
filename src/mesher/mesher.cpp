#include "mesher.h"

#include "geometry/model.h"
#include "geometry/hole.h"
#include "geometry/part.h"
#include "geometry/region.h"
#include "geometry/segment.h"
#include "geometry/vertex.h"

#include "mesh/edge.h"
#include "mesh/mesh.h"
#include "mesh/node.h"
#include "mesh/triangle.h"
#include "mesh/tetrahedron.h"

#include "xml/xmlutils.h"

#include <boost/qvm/vec_operations.hpp>

#include <fstream>
#include <iostream>
#include <set>

#include <tetgen.h>

namespace morfeus {
namespace mesher {

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

//
// Adds the facets for the tetgenio data strcuture
//
// The specification of facets is more complicated than points,
// holes, or regions. Tetgen requires that all polygons in the geometry
// that are located in the same plane (i.e. coplanar) be part of the
// same facet in the geometry. So, we must determine any polygons
// in the mesh that are coplanar and assign them to a single facet.
//
// In addition, every polygon shape uses local numbers for
// labelling it's vertices. Therefore, we must provide a mapping
// from the local number of each shape to the global number of
// the geometry.
//
void Mesher::addFacets(const geometry::Model *geometry, tetgenio *in) const
{
//  int32_t vertexOffset = 0;

//  // Make a list of all mesher polygons from the geometry
//  std::vector<Mesher::Polygon> polygons;
//  for (std::size_t i = 0; i < geometry->totalShapes(); i++) {
//    const geometry::Shape * shape = geometry->shape(i);

//    // The global specification ::Facet below is required to
//    // make sure the compiler doesn't try to use Mesher::Facet
//    std::vector<geometry::Vertex> vertexList = shape->getVertexList();
//    std::vector<geometry::Face> facetList = shape->getFacetList();
//    for (std::size_t j = 0; j < facetList.size(); j++) {
//      const geometry::Face & facet = facetList.at(j);

//      Mesher::Polygon p;
//      for (std::size_t k = 0; k < facet.totalSegments(); k++) {
//        const geometry::Segment & segment = facet.segment(k);
//        int32_t node1 = segment.node1();
//        geometry::Vertex v1 = vertexList.at(node1);
//        v1.setNumber(v1.number()+vertexOffset);
//        p.addVertex(v1);
//      }
//      polygons.push_back(p);
//    }

//    vertexOffset += static_cast<int32_t>(vertexList.size());
//  }

//  // Determine the number of facets taking into account the coplanar polygons
//  std::vector<Mesher::Facet> facets;
//  for (std::size_t i = 0; i < polygons.size(); i++) {
//    const Mesher::Polygon & p1 = polygons.at(i);

//    Mesher::Facet facet;
//    facet.addPolygon(p1);

//    // Figure out which other polygons, if any, are coplanar to this one
//    std::vector<std::size_t> ids;
//    for (std::size_t j = i+1; j < polygons.size(); j++) {
//      const Mesher::Polygon & p2 = polygons.at(j);
//      if (p1.isCoplanar(p2)) {
//        ids.push_back(j);
//        facet.addPolygon(p2);
//      }
//    }

//    // Remove any coplanar polygons from the list
//    // since they are already included in a facet
//    while (ids.size() > 0) {
//      std::size_t id = ids.back();
//      polygons.erase(polygons.begin()+id);
//      ids.pop_back();
//    }

//    // Add this facet to the list
//    facets.push_back(facet);
//  }

//  // Allocate the facets
//  in->numberoffacets = static_cast<int>(facets.size());
//  if (in->numberoffacets > 0) {
//    in->facetlist = new tetgenio::facet[in->numberoffacets];
//    in->facetmarkerlist = new int[in->numberoffacets];
//  }

//  // Write the facets to the input structure
//  for (std::size_t i = 0; i < in->numberoffacets; i++) {
//    const Mesher::Facet & facet = facets.at(i);
//    tetgenio::facet * f = &in->facetlist[i];

//    f->numberofholes = 0;
//    f->holelist = nullptr;

//    f->numberofpolygons = static_cast<int>(facet.totalPolygons());
//    f->polygonlist = new tetgenio::tetgenio::polygon[f->numberofpolygons];

//    for (std::size_t j = 0; j < facet.totalPolygons(); j++) {
//      const Mesher::Polygon & polygon = facet.polygon(j);
//      tetgenio::polygon * p = &f->polygonlist[j];
//      p->numberofvertices = static_cast<int>(polygon.totalVertices());
//      p->vertexlist = new int[p->numberofvertices];
//      for (std::size_t k = 0; k < polygon.totalVertices(); k++) {
//        const geometry::Vertex & v = polygon.vertex(k);
//        p->vertexlist[k] = v.number();
//      }
//    }
//  }
}

//// Add the holes to the tetgenio structure
////
//// This is fairly straigtforward. The only slight complication
//// is that we must properly index the values into the holelist strcture
//// Essentially, for each hole there are three numbers, the x, y, and z
//// position of the hole. So, for each hole we just increment our
//// counter by 3.
//void Mesher::addHoles(const geometry::Model *model, tetgenio *in) const
//{
//  // Allocate holes
//  in->numberofholes = static_cast<int>(model->totalHoles());
//  if (in->numberofholes > 0) {
//    in->holelist = new double[in->numberofholes*3];

//    // Write holes to input structure
//    std::size_t holeIndex = 0;
//    for (std::size_t i = 0; i < model->totalHoles(); i++) {
//      const geometry::Hole * hole = model->hole(i);
//      in->holelist[holeIndex] = hole->position().x();
//      in->holelist[holeIndex+1] = hole->position().y();
//      in->holelist[holeIndex+2] = hole->position().z();
//      holeIndex += 3;
//    }
//  }
//}

// Add the tet elements created by tetgen to the mesh
void Mesher::addMeshElements(const tetgenio *out, mesh::Mesh *mesh) const
{
  std::size_t tetIndex = 0;
  for (std::size_t i = 0; i < out->numberoftetrahedra; i++) {
    std::unique_ptr<mesh::Tetrahedron> tet(new mesh::Tetrahedron(static_cast<int32_t>(i)));
    int32_t attribute = out->tetrahedronattributelist[i];
    for (std::size_t j = 0; j < out->numberofcorners; j++) {
      tet->setNode(j, out->tetrahedronlist[tetIndex+j]);
    }
    tet->setAttribute(attribute);
    mesh->addElement(std::move(tet));
    tetIndex += out->numberofcorners;
  }
}

// Add the nodes created by tetgen to the mesh
void Mesher::addMeshNodes(const tetgenio *out, mesh::Mesh *mesh) const
{
  std::size_t pointIndex = 0;
  for (std::size_t i = 0; i < out->numberofpoints; i++) {
    double x = out->pointlist[pointIndex];
    double y = out->pointlist[pointIndex+1];
    double z = out->pointlist[pointIndex+2];

    std::unique_ptr<mesh::Node> node(new mesh::Node(static_cast<int32_t>(i), x, y, z));
    mesh->addNode(std::move(node));
    pointIndex += 3;
  }
}

// Add the points to the tetgenio structure
//
// Each point in the tetgen point list is specified by three values
// the x, y, and z location of the point. So for each point we need
// to increment a counter by three to keep track of the proper index.
//
// In addition, there is a point attribute list and a point marker list
// Currently, these are unused but we must still allocate space
// for the marker list and set the number of attributes to 0.
//
void Mesher::addPoints(const geometry::Model *model, tetgenio *in) const
{
//  // Determine the number of points for all shapes
//  in->numberofpointattributes = 0;
//  std::vector<geometry::Vertex> allVertexList;
//  for (std::size_t i = 0; i < model->totalShapes(); i++) {
//    const geometry::Shape * shape = model->shape(i);
//    std::vector<geometry::Vertex> vertexList = shape->getVertexList();
//    for (std::size_t j = 0; j < vertexList.size(); j++)
//      allVertexList.push_back(vertexList.at(j));
//  }
//  in->numberofpoints = static_cast<int>(allVertexList.size());

//  // Allocate points
//  if (in->numberofpoints > 0) {
//    in->pointlist = new double[static_cast<uint64_t>(in->numberofpoints*3)];
//    in->pointmarkerlist = new int[in->numberofpoints];
//  }

//  // Write points to input structure
//  std::size_t pointIndex = 0;
//  std::size_t pointMarkerIndex = 0;
//  int32_t vertexOffset = 0;
//  for (std::size_t j = 0; j < allVertexList.size(); j++) {
//    const geometry::Vertex & v = allVertexList.at(j);
//    in->pointlist[pointIndex] = v.x();
//    in->pointlist[pointIndex+1] = v.y();
//    in->pointlist[pointIndex+2] = v.z();
//    in->pointmarkerlist[pointMarkerIndex] = 0;
//    pointIndex += 3;
//    pointMarkerIndex++;
//  }
}

// Add the regions to the tetgenio structure
//
// This is fairly straigtforward. The only slight complication
// is that we must properly index the values into the regionlist strcture
// Essentially, for each region there are five values, the x, y, and z
// position of the region along with an attrbute and maximum tet area.
// So, for each hole we just increment our counter by 5.
//
//void Mesher::addRegions(const geometry::Model *model, tetgenio *in) const
//{
//  // Allocate regions
//  in->numberofregions = static_cast<int>(model->totalRegions());
//  if (in->numberofregions > 0) {
//    in->regionlist = new double[in->numberofregions*5];

//    // Write regions to input structure
//    std::size_t regionIndex = 0;
//    for (std::size_t i = 0; i < model->totalRegions(); i++) {
//      const geometry::Region * region = model->region(i);
//      in->regionlist[regionIndex] = region->position().x();
//      in->regionlist[regionIndex+1] = region->position().y();
//      in->regionlist[regionIndex+2] = region->position().z();
//      in->regionlist[regionIndex+3] = region->attribute();
//      in->regionlist[regionIndex+4] = region->maxArea();
//      regionIndex += 5;
//    }
//  }
//}

//! Creates the volume mesh from the geometry specification
//!
//! This function creates the volume mesh from the geometry specification
//! The mesh generation is done using the tetgen library which requires the
//! geometry be formatted in a particular way.
//!
//! Tetgen requires a list of nodes, a list of facets (composed of one or
//! more polygons and holes), a list of holes not associated with polygons, and
//! a list of regions that may contain attributes and/or area constraints.
//!
//! The general algorithm is as follows:
//! 1. Identify all the polygonal shapes in the of the geometry
//! 2. Identify multiple shapes in the same plane that must belong to the same facet in tetgen.
//! 3. Populate the tetgenio input data structure with the nodes and facets
//! 4. Populate the tetgenio input data structure with any other holes and regions
//! 5. Call tetrahedralize to create the volume mesh
//! 6. Copy the relevant data from the tetgenio output data structure to the appropriate mesh format
//!
void Mesher::createMesh(const geometry::Model * model, mesh::Mesh * mesh) const
{
  // Populate the input structure
  tetgenio in;
  addPoints(model, &in);
  addFacets(model, &in);
  //addHoles(model, &in);
  //addRegions(model, &in);
  writePolyFile("temp.poly", &in);

  // Run the tetgen mesher to populate the output structure
  tetgenio out;
  char switches[] = "pqaAQ";
  tetrahedralize(switches, &in, &out);

  // Populate the mesh
  addMeshNodes(&out, mesh);
  addMeshElements(&out, mesh);
  mesh->writeVtkFile("morfeus.vtk");

  std::size_t index = 0;
  std::cout << out.numberoftrifaces << "\n";
  for (std::size_t i = 0; i < out.numberoftrifaces; i++) {
    std::cout << out.trifacemarkerlist[i] << "\n";
    for (std::size_t j = 0; j < 3; j++) {
      //std::cout << out.trifacelist[index+j] << " ";
    }
    std::cout << "\n";
    index += 3;
  }
}

//! Print the mesher information to the output stream
void Mesher::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Mesher");
  xmlutils::printValue(output, tabPos+2, "Min Edge Length: ", mMinEdgeLength);
  xmlutils::printValue(output, tabPos+2, "Max Edge Length: ", mMaxEdgeLength);
  xmlutils::printValue(output, tabPos+2, "Min Triangle Area: ", mMinTriangleArea);
  xmlutils::printValue(output, tabPos+2, "Max Triangle Area: ", mMaxTriangleArea);
  xmlutils::printHeader(output, tabPos, "End Mesher");
  output << "\n";
}

//! Read the mesher specification from a XML file
void Mesher::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
  setMinEdgeLength(xmlutils::readAttribute<double>(node, "min-edge-length"));
  setMaxEdgeLength(xmlutils::readAttribute<double>(node, "max-edge-length"));
  setMinTriangleArea(xmlutils::readAttribute<double>(node, "min-triangle-area"));
  setMaxTriangleArea(xmlutils::readAttribute<double>(node, "max-triangle-area"));
}

//! Write the mesher specification to a XML file
void Mesher::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "min-edge-length", mMinEdgeLength);
  xmlutils::writeAttribute(document, node, "max-edge-length", mMaxEdgeLength);
  xmlutils::writeAttribute(document, node, "min-triangle-area", mMinTriangleArea);
  xmlutils::writeAttribute(document, node, "max-triangle-area", mMaxTriangleArea);
}

void Mesher::writePolyFile(const std::string &fileName, const tetgenio *in) const
{
  std::ofstream output(fileName);

  output << "# points\n";
  output << in->numberofpoints << " 3\n";
  output << "# Node index, coordinates\n";
  int pointIndex = 0;
  for (int i = 0; i < in->numberofpoints; i++) {
    output << i << " " << in->pointlist[pointIndex] << " " << in->pointlist[pointIndex+1] << " " << in->pointlist[pointIndex+2] << "\n";
    pointIndex += 3;
  }

  output << "# Facet list\n";
  output << "# total facets\n";
  output << in->numberoffacets << " 0\n";
  output << "# Facets\n";
  for (int i = 0; i < in->numberoffacets; i++) {
    const tetgenio::facet * f = &in->facetlist[i];
    output << f->numberofpolygons << " " << f->numberofholes << "\n";
    for (int j = 0; j < f->numberofpolygons; j++) {
      const tetgenio::polygon * p = &f->polygonlist[j];
      output << p->numberofvertices << " ";
      for (int k = 0; k < p->numberofvertices; k++) {
        output << p->vertexlist[k] << " ";
      }
      output << "\n";
    }
  }

  output << "# holes\n";
  output << in->numberofholes << "\n";
  int holeIndex = 0;
  for (int i = 0; i < in->numberofholes; i++) {
    output << i << " " << in->holelist[holeIndex] << " " << in->holelist[holeIndex+1] << " " << in->holelist[holeIndex+2] << "\n";
    holeIndex += 3;
  }

  output << "# regions\n";
  output << in->numberofregions << "\n";
  int regionIndex = 0;
  for (int i = 0; i < in->numberofregions; i++) {
    output << i << " "
           << in->regionlist[regionIndex] << " "
           << in->regionlist[regionIndex+1] << " "
           << in->regionlist[regionIndex+2] << " "
           << in->regionlist[regionIndex+3] << " "
           << in->regionlist[regionIndex+4] << "\n";

    regionIndex += 5;
  }
}

void Mesher::Facet::addPolygon(const Mesher::Polygon &polygon)
{
  mPolygons.push_back(polygon);
}

const Mesher::Polygon & Mesher::Facet::polygon(std::size_t index) const
{
  return mPolygons.at(index);
}

std::size_t Mesher::Facet::totalPolygons() const
{
  return mPolygons.size();
}

void Mesher::Polygon::addVertex(const geometry::Vertex &vertex)
{
  mVertices.push_back(vertex);
}

void Mesher::Polygon::clearVertices()
{
  mVertices.clear();
}

boost::qvm::vec<double, 3> Mesher::Polygon::computeNormal() const
{
  assert(totalVertices() >= 3);

  double v[3];
  double v1[3], v2[3], v3[3], v4[3], v5[3];

  v1[0] = mVertices[0].x();
  v1[1] = mVertices[0].y();
  v1[2] = mVertices[0].z();
  v2[0] = mVertices[1].x();
  v2[1] = mVertices[1].y();
  v2[2] = mVertices[1].z();
  v3[0] = mVertices[2].x();
  v3[1] = mVertices[2].y();
  v3[2] = mVertices[2].z();

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
bool Mesher::Polygon::isCoplanar(const Mesher::Polygon & polygon, double tolerance) const
{
  const geometry::Vertex & v1 = this->mVertices[0];

  // First determine the normal vector using the first three points of this polygon
  boost::qvm::vec<double, 3> nv = computeNormal();

  // Next determine if each point in the supplied polygon is coplanar with this polygon.
  for (std::size_t i = 0; i < polygon.totalVertices(); i++) {
    const geometry::Vertex & v2 = polygon.vertex(i);

    // Compute the vector from the first point of this polygon to the current point
    boost::qvm::vec<double, 3> vd = {v2.x()-v1.x(), v2.y()-v1.y(), v2.z()-v1.z()} ;

    // Compute the dot product of the vector v to the normal vector n
    double dot = boost::qvm::dot(nv, vd);
    if (fabs(dot) >= tolerance)
      return false;
  }

  return true;
}

std::size_t Mesher::Polygon::totalVertices() const
{
  return mVertices.size();
}

const geometry::Vertex & Mesher::Polygon::vertex(std::size_t index) const
{
  return mVertices.at(index);
}

}
}
