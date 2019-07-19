#ifndef DIRECTSOLVER_H
#define DIRECTSOLVER_H

#include "solver.h"

class DirectSolver
    : public Solver
{
public:
  MORFEUS_LIB_DECL DirectSolver();

protected:
  void allocateMatrices(const MeshInformation * meshInfo) override;
  void clearMatrices(const Mesh * mesh, const MeshInformation * meshInfo) override;
  virtual void updateBoundaryIntegralMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2) override;
  virtual void updateFiniteElementMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2) override;
  vector solveSystem(const vector & rhs) override;
};

#endif // DIRECTSOLVER_H
