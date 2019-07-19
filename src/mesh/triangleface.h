#ifndef TRIANGLEFACE_H
#define TRIANGLEFACE_H

#include "face.h"

class TriangleFace
    : public Face
{
public:
  TriangleFace(int32_t id);

protected:
  double doComputeArea(const Mesh * mesh) const override;
  dcomplex doComputeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const override;
  bool doIntersects(const Face * face) const override;
};

#endif // TRIANGLEFACE_H
