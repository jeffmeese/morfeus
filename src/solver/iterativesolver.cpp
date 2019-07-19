#include "iterativesolver.h"

#include "edge.h"
#include "element.h"
#include "mesh.h"
#include "meshinformation.h"

IterativeSolver::IterativeSolver()
  : mAlgorithm(BiCG)
  , mMinIterations(1)
  , mMaxIterations(100)
  , mTolerance(0.001)
{
}

IterativeSolver::IterativeSolver(Algorithm algorithm, std::size_t minIterations, std::size_t maxIterations, double tolerance)
  : mAlgorithm(algorithm)
  , mMinIterations(minIterations)
  , mMaxIterations(maxIterations)
  , mTolerance(tolerance)
{
}

void IterativeSolver::allocateMatrices(const MeshInformation * meshInfo)
{
  std::size_t boundaryUnknowns = meshInfo->totalBoundaryUnknowns();
  std::size_t totalUnknowns = meshInfo->totalUnknowns();

  mFeMatrix.resize(totalUnknowns, totalUnknowns);
  mBiMatrix.resize(boundaryUnknowns, boundaryUnknowns);
}

Solver::vector IterativeSolver::bicg(const vector & rhs)
{
  vector efield;
  return efield;
}

Solver::vector IterativeSolver::bicgStab(const vector & rhs)
{
  vector efield;
  return efield;
}

Solver::vector IterativeSolver::cgs(const vector & rhs)
{
  vector efield;
  return efield;
}

void IterativeSolver::clearMatrices(const Mesh *mesh, const MeshInformation *meshInfo)
{
  for (std::size_t i = 1; i <= mesh->totalElements(); i++) {
    const Element * element = mesh->element(i);
    for (std::size_t j = 1; j <= element->totalEdges(); j++) {
      const Edge * sourceEdge = mesh->edge(element->edge(j));
      int32_t sourceUnknown = sourceEdge->unknownNumber();
      for (std::size_t k = 1; k <= element->totalEdges(); k++) {
        const Edge * testEdge = mesh->edge(element->edge(k));
        int32_t testUnknown = testEdge->unknownNumber();

        if (sourceUnknown >= 0 && sourceUnknown <= testUnknown) {
          std::size_t row = sourceUnknown-1;
          std::size_t col = testUnknown-1;

          mFeMatrix(row, col) = dcomplex(0.0,0.0);
          mFeMatrix(col, row) = dcomplex(0.0,0.0);
         }
      }
    }
  }

  for (std::size_t i = 0; i < meshInfo->totalBoundaryUnknowns(); i++) {
    for (std::size_t j = i; j < meshInfo->totalBoundaryUnknowns(); j++) {
      mBiMatrix(i, j) = dcomplex(0.0,0.0);
    }
  }
}

void IterativeSolver::updateBoundaryIntegralMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2)
{
  mBiMatrix(row, col) += i1 + i2;
}

void IterativeSolver::updateFiniteElementMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2)
{
  mFeMatrix(row, col) += i1 + i2;
}

Solver::vector IterativeSolver::solveSystem(const vector &rhs)
{
  vector efield;
  switch (mAlgorithm) {
  case BiCG:
    efield = bicg(rhs);
    break;
  case BiCGStab:
    efield = bicgStab(rhs);
    break;
  case CGS:
    efield = cgs(rhs);
    break;
  }
  return efield;
}
