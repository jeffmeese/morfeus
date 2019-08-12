#include "rectangle.h"

#include "mesh.h"

namespace morfeus {
namespace mesh {

static const std::string OBJECT_ID("Rectangle");

Rectangle::Rectangle()
  : Face(OBJECT_ID, 4, 4)
{
}

Rectangle::Rectangle(int32_t number)
  : Face(OBJECT_ID, number, 4, 4)
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

math::dcomplex Rectangle::doComputeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const
{
  math::dcomplex value;
  return value;
}

math::dcomplex Rectangle::doComputePlanewaveEntry(std::size_t edge, double freq, double alpha, double theta, double phi, const Mesh *mesh) const
{
  math::dcomplex value(0.0,0.0);
  return value;
}

}
}
