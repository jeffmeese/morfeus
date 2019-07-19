#include "triangleface.h"

#include "mesh.h"

TriangleFace::TriangleFace(int32_t id)
  : Face(id, 3, 3)
{
}

double TriangleFace::doComputeArea(const Mesh * mesh) const
{
  return 0.0;
}

dcomplex TriangleFace::doComputeMomEntry(const Face *otherFace, const Mesh * mesh, std::size_t localEdge) const
{
  dcomplex value(0.0,0.0);
  return value;
}

bool TriangleFace::doIntersects(const Face *face) const
{
  return false;
}
