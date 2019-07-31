#include "rectangle.h"

#include "mesh.h"

namespace morfeus {
namespace mesh {

Rectangle::Rectangle()
  : Face("RectangleFace", 4, 4)
{
}

Rectangle::Rectangle(int32_t number)
  : Face("RectangleFace", number, 4, 4)
{

}

//dcomplex RectangleFace::computeEntry(const RectangleFace * otherFace, const Mesh & mesh, std::size_t localEdge) const
//{
//  dcomplex result(0.0,0.0);
//  return result;
//}

//dcomplex RectangleFace::computeEntry(const TriangleFace * otherFace, const Mesh & mesh, std::size_t localEdge) const
//{
//  dcomplex result(0.0,0.0);
//  return result;
//}

void Rectangle::doPrint(std::ostream &output, int tabPos) const
{
}

void Rectangle::doXmlRead(rapidxml::xml_document<> &, rapidxml::xml_node<> * node)
{
}

void Rectangle::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
}

double Rectangle::doComputeArea(const Mesh * mesh) const
{
  return 0.0;
}

dcomplex Rectangle::doComputeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const
{
  dcomplex value;
  return value;
}

dcomplex Rectangle::doComputePlanewaveEntry(std::size_t edge, double freq, double alpha, double theta, double phi, const Mesh *mesh) const
{
  dcomplex value(0.0,0.0);
  return value;
}

}
}
