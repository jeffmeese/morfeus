#include "rectangleface.h"

#include "mesh.h"

RectangleFace::RectangleFace(int32_t id)
  : Face(id, 4, 4)
{

}

dcomplex RectangleFace::computeEntry(const RectangleFace * otherFace, const Mesh & mesh, int32_t localEdge) const
{
  dcomplex result(0.0,0.0);
  return result;
}

dcomplex RectangleFace::computeEntry(const TriangleFace * otherFace, const Mesh & mesh, int32_t localEdge) const
{
  dcomplex result(0.0,0.0);
  return result;
}

double RectangleFace::doComputeArea(const Mesh & mesh) const
{
  return 0.0;
}


