#include "directsolver.h"

#include "meshinformation.h"

DirectSolver::DirectSolver()
{

}

void DirectSolver::allocateMatrices(const MeshInformation * meshInfo)
{
  std::size_t boundaryUnknowns = meshInfo->totalBoundaryUnknowns();
  std::size_t totalUnknowns = meshInfo->totalUnknowns();
}

void DirectSolver::clearMatrices(const Mesh *mesh, const MeshInformation *meshInfo)
{

}

void DirectSolver::doPrint(std::ostream & output, int tabPos) const
{

}

void DirectSolver::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{

}

void DirectSolver::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{

}

Solver::vector DirectSolver::solveSystem(const vector & rhs)
{
  vector efield;
  return efield;
}

void DirectSolver::updateBoundaryIntegralMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2)
{

}

void DirectSolver::updateFiniteElementMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2)
{

}

