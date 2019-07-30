#ifndef TRIANGLEFACE_H
#define TRIANGLEFACE_H

#include "face.h"

namespace Morfeus {
namespace mesh {

class TriangleFace
    : public Face
{
public:
  TriangleFace();
  TriangleFace(int32_t id);

protected:
  double doComputeArea(const Mesh * mesh) const override;
  dcomplex doComputeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const override;
  dcomplex doComputePlanewaveEntry(std::size_t edge, double freq, double alpha, double theta, double phi, const Mesh * mesh) const override;
};

}
}

#endif // TRIANGLEFACE_H
