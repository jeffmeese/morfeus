#include "directsolver.h"

#include "meshinformation.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {

static const std::string OBJECT_ID("Direct Solver");

DirectSolver::DirectSolver()
  : Solver (OBJECT_ID)
{
}

void DirectSolver::allocateMatrices(const MeshInformation * meshInfo)
{
  std::size_t boundaryUnknowns = meshInfo->totalBoundaryUnknowns();
  std::size_t totalUnknowns = meshInfo->totalUnknowns();
}

void DirectSolver::clearMatrices(const mesh::Mesh *mesh, const MeshInformation *meshInfo)
{

}

void DirectSolver::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, OBJECT_ID);
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

namespace  {
  const bool r = Solver::factory().registerType(OBJECT_ID, boost::bind(boost::factory<DirectSolver*>()));
}

}
