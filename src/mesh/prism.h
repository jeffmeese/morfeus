#ifndef MORFEUS_MESH_PRISM_H
#define MORFEUS_MESH_PRISM_H

#include "morfeus.h"
#include "element.h"

namespace morfeus {
  namespace mesh {
    class Triangle;
  }
}

namespace morfeus {
namespace mesh {

class Prism
    : public Element
{
public:
  MORFEUS_LIB_DECL Prism();
  MORFEUS_LIB_DECL Prism(int32_t number);

protected:
  void doComputeFeEntry(const Mesh * mesh, std::size_t localEdge1, std::size_t localEdge2, dcomplex & i1, dcomplex & i2) const override;
  void doEdgeNodes(std::size_t index, int32_t & localNode1, int32_t & localNode2) const override;
  Face * doGetFace(std::size_t index) const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void edgeConstants(std::size_t edge, std::size_t & k1, std::size_t & k2) const;
  double ixs(double a, double const * x, double const * y) const;
  double iys(double a, double const * x, double const * y) const;
  double ixxs(double a, double b, double const * x, double const * y) const;
  double iyys(double a, double b, double const * x, double const * y) const;
  double izzl(double zl, double zu, double a, double b) const;
  double sxx(double const * x, double const * y) const;
  double sxy(double const * x, double const * y) const;
  double syy(double const * x, double const * y) const;

private:
  void createFaces();

private:
  typedef std::unique_ptr<Triangle> TrianglePtr;

private:
  std::vector<TrianglePtr> mFaces;
};

}
}

#endif // MORFEUS_MESH_PRISM_H
