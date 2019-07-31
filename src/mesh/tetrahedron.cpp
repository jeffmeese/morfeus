#include "tetrahedron.h"
#include "triangle.h"

namespace morfeus {
namespace mesh {

Tetrahedron::Tetrahedron()
  : Element("TetElement", 4, 6, 4)
{
  createFaces();
}

Tetrahedron::Tetrahedron(int32_t number)
  : Element("TetElement", number, 4, 6, 4)
{
  createFaces();
}

void Tetrahedron::createFaces()
{
  mFaces.push_back(TrianglePtr(new Triangle));
  mFaces.push_back(TrianglePtr(new Triangle));
  mFaces.push_back(TrianglePtr(new Triangle));
  mFaces.push_back(TrianglePtr(new Triangle));
}

void Tetrahedron::doComputeFeEntry(const Mesh * mesh, std::size_t localEdge1, std::size_t localEdge2, dcomplex & i1, dcomplex & i2) const
{

}

Face * Tetrahedron::doGetFace(std::size_t index) const
{
  static const int node1[] = {0, 1, 0, 0};
  static const int node2[] = {1, 2, 2, 1};
  static const int node3[] = {2, 3, 3, 3};
  static const int edge1[] = {0, 1, 2, 0};
  static const int edge2[] = {1, 4, 4, 3};
  static const int edge3[] = {2, 3, 3, 5};

  Triangle * face = mFaces.at(index).get();
  face->setNode(0, node(node1[index]));
  face->setNode(1, node(node2[index]));
  face->setNode(2, node(node3[index]));
  face->setEdge(0, edge(edge1[index]));
  face->setEdge(1, edge(edge2[index]));
  face->setEdge(2, edge(edge3[index]));

  return face;
}

void Tetrahedron::doEdgeNodes(std::size_t index, int32_t & localNode1, int32_t & localNode2) const
{
  static const int edge_node1[] = {0, 1, 2, 1, 2, 0};
  static const int edge_node2[] = {1, 2, 0, 3, 3, 3};
  localNode1 = edge_node1[index];
  localNode2 = edge_node2[index];
}

void Tetrahedron::doPrint(std::ostream &output, int tabPos) const
{
}

void Tetrahedron::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
}

void Tetrahedron::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
}


}
}
