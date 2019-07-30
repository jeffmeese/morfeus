#ifndef RECTANGLEFACE_H
#define RECTANGLEFACE_H

#include "face.h"

namespace Morfeus {
namespace mesh {

class TriangleFace;

class RectangleFace
    : public Face
{
public:
  MORFEUS_LIB_DECL RectangleFace();
  MORFEUS_LIB_DECL RectangleFace(int32_t number);

public:
  //MORFEUS_LIB_DECL dcomplex computeEntry(const RectangleFace * otherFace, const Mesh & mesh, std::size_t localEdge) const;
  //MORFEUS_LIB_DECL dcomplex computeEntry(const TriangleFace * otherFace, const Mesh & mesh, std::size_t localEdge) const;

protected:
  double doComputeArea(const Mesh * mesh) const override;
  dcomplex doComputeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const override;
  dcomplex doComputePlanewaveEntry(std::size_t edge, double freq, double alpha, double theta, double phi, const Mesh * mesh) const override;
};

}
}

#endif // RECTANGLEFACE_H
