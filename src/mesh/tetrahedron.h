#ifndef MORFEUS_MESH_TETRAHEDRON_H
#define MORFEUS_MESH_TETRAHEDRON_H

#include "element.h"

namespace morfeus {
  namespace mesh {
    class Triangle;
  }
}

namespace morfeus {
namespace mesh {

class Tetrahedron
    : public Element
{
public:
  MORFEUS_LIB_DECL Tetrahedron();
  MORFEUS_LIB_DECL Tetrahedron(int32_t number);

protected:
  void doComputeFeEntry(const Mesh * mesh, std::size_t localEdge1, std::size_t localEdge2, dcomplex & i1, dcomplex & i2) const override;
  void doEdgeNodes(std::size_t index, int32_t & localNode1, int32_t & localNode2) const override;
  Face * doGetFace(std::size_t index) const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void createFaces();

private:
  typedef std::unique_ptr<Triangle> TrianglePtr;

private:
  std::vector<TrianglePtr> mFaces;
};

}
}

#endif // MORFEUS_MESH_TETRAHEDRON_H
