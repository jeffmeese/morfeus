#ifndef MORFEUS_SOLUTION_DIRECTSOLVER_H
#define MORFEUS_SOLUTION_DIRECTSOLVER_H

#include "solver.h"

namespace morfeus {
namespace solution {

class DirectSolver
    : public Solver
{
public:
  MORFEUS_LIB_DECL DirectSolver();

protected:
  void allocateMatrices(const MeshInformation * meshInfo) override;
  void clearMatrices(const mesh::Mesh * mesh, const MeshInformation * meshInfo) override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;
  virtual void updateMatrix(std::size_t row, std::size_t col, const math::dcomplex & i1, const math::dcomplex & i2) override;
  math::vector solveSystem(const math::vector & rhs) override;
};

}
}

#endif // MORFEUS_SOLUTION_DIRECTSOLVER_H
