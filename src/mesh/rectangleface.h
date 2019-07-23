#ifndef RECTANGLEFACE_H
#define RECTANGLEFACE_H

#include "face.h"

class TriangleFace;

class RectangleFace
    : public Face
{
public:
  RectangleFace(int32_t id);

public:
  //MORFEUS_LIB_DECL dcomplex computeEntry(const RectangleFace * otherFace, const Mesh & mesh, std::size_t localEdge) const;
  //MORFEUS_LIB_DECL dcomplex computeEntry(const TriangleFace * otherFace, const Mesh & mesh, std::size_t localEdge) const;

protected:
  double doComputeArea(const Mesh * mesh) const override;
  dcomplex doComputeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const override;
  dcomplex doComputePlanewaveEntry(std::size_t edge, double freq, const Planewave * planewave, const Mesh * mesh) const override;
};

#endif // RECTANGLEFACE_H
