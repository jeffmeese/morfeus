#ifndef MORFEUS_MESH_TRIANGLE_H
#define MORFEUS_MESH_TRIANGLE_H

#include "face.h"

namespace morfeus {
namespace mesh {

class Triangle
    : public Face
{
public:
  Triangle();
  Triangle(int32_t id);

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;
  double doComputeArea(const Mesh * mesh) const override;
  math::dcomplex doComputeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const override;
  math::dcomplex doComputePlanewaveEntry(std::size_t edge, double freq, double alpha, double theta, double phi, const Mesh * mesh) const override;
};

}
}

#endif // MORFEUS_MESH_TRIANGLE_H
