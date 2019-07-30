#ifndef DIRECTSOLVER_H
#define DIRECTSOLVER_H

#include "solver.h"

namespace morfeus {

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
  virtual void updateBoundaryIntegralMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2) override;
  virtual void updateFiniteElementMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2) override;
  vector solveSystem(const vector & rhs) override;
};

}

#endif // DIRECTSOLVER_H
