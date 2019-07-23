#include "rectangleface.h"

#include "mesh.h"

RectangleFace::RectangleFace(int32_t id)
  : Face(id, 4, 4)
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

double RectangleFace::doComputeArea(const Mesh * mesh) const
{
  return 0.0;
}

dcomplex RectangleFace::doComputeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const
{
  dcomplex value;
  return value;
}

dcomplex RectangleFace::doComputePlanewaveEntry(std::size_t edge, double freq, const Planewave *planewave, const Mesh *mesh) const
{
  dcomplex value(0.0,0.0);
  return value;
}
