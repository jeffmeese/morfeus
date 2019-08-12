#include "directsolver.h"

#include "meshinformation.h"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace solution {

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
}

void DirectSolver::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{

}

void DirectSolver::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{

}

math::vector DirectSolver::solveSystem(const math::vector & rhs)
{
  math::vector efield;
  return efield;
}

void DirectSolver::updateMatrix(std::size_t row, std::size_t col, const math::dcomplex & i1, const math::dcomplex & i2)
{

}

namespace  {
  const bool r = Solver::factory().registerType(OBJECT_ID, boost::bind(boost::factory<DirectSolver*>()));
}

}
}
