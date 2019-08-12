#include "iterativesolver.h"

#include "meshinformation.h"

#include "mesh/edge.h"
#include "mesh/element.h"
#include "mesh/mesh.h"

#include "xml/xmlutils.h"

#include "Eigen/IterativeLinearSolvers"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

namespace morfeus {
namespace solution {

static const std::string OBJECT_ID("Iterative Solver");

IterativeSolver::IterativeSolver()
  : Solver(OBJECT_ID)
  , mAlgorithm(BiCGStab)
  , mMaxIterations(100)
  , mMinIterations(1)
  , mTolerance(0.001)
{
}

IterativeSolver::IterativeSolver(Algorithm algorithm, std::size_t minIterations, std::size_t maxIterations, double tolerance)
  : Solver(OBJECT_ID)
  , mAlgorithm(algorithm)
  , mMaxIterations(maxIterations)
  , mMinIterations(minIterations)
  , mTolerance(tolerance)
{
}

void IterativeSolver::allocateMatrices(const MeshInformation * meshInfo)
{
  mMatrix.resize(meshInfo->totalUnknowns(), meshInfo->totalUnknowns());
}

math::vector IterativeSolver::bicg(const math::vector & rhs)
{
  math::vector efield;
  return efield;
}

math::vector IterativeSolver::bicgStab(const math::vector & rhs)
{
  Eigen::BiCGSTAB<math::sparse_matrix> solver;
  solver.setMaxIterations(mMaxIterations);
  solver.setTolerance(mTolerance);
  solver.compute(mMatrix);
  math::vector efield = solver.solve(rhs);
  return efield;
}

math::vector IterativeSolver::cgs(const math::vector & rhs)
{
  Eigen::ConjugateGradient<math::sparse_matrix> solver;
  solver.setMaxIterations(mMaxIterations);
  solver.setTolerance(mTolerance);
  solver.compute(mMatrix);
  math::vector efield = solver.solve(rhs);
  return efield;
}

void IterativeSolver::clearMatrices(const mesh::Mesh *mesh, const MeshInformation *meshInfo)
{
  mMatrix.setZero();
}

void IterativeSolver::doPrint(std::ostream & output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "Algorithm: ", formatAlgoString(mAlgorithm));
  xmlutils::printValue(output, tabPos, "Min Iterations: ", mMinIterations);
  xmlutils::printValue(output, tabPos, "Max Iterations: ", mMaxIterations);
  xmlutils::printValue(output, tabPos, "Tolerance: ", mTolerance);
}

void IterativeSolver::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setAlgorithm(readAlgorithmAttribute(node));
  setMinIterations(xmlutils::readAttribute<int32_t>(node, "min-iterations"));
  setMaxIterations(xmlutils::readAttribute<int32_t>(node, "max-iterations"));
  setTolerance(xmlutils::readAttribute<double>(node, "tolerance"));
}

void IterativeSolver::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "algorithm", formatAlgoString(mAlgorithm));
  xmlutils::writeAttribute(document, node, "min-iterations", mMinIterations);
  xmlutils::writeAttribute(document, node, "max-iterations", mMaxIterations);
  xmlutils::writeAttribute(document, node, "tolerance", mTolerance);
}

std::string IterativeSolver::formatAlgoString(Algorithm algorithm) const
{
  if (algorithm == CGS)
    return "cgs";
  else if (algorithm == BiCGStab)
    return "bicg-stab";

  return "Unknown";
}

IterativeSolver::Algorithm IterativeSolver::readAlgorithmAttribute(rapidxml::xml_node<> *node) const
{
  std::string algoString = xmlutils::readAttribute<std::string>(node, "algorithm");

  Algorithm algorithm = BiCGStab;
  if (algoString == "cgs")
    algorithm = CGS;

  return algorithm;
}

void IterativeSolver::updateMatrix(std::size_t row, std::size_t col, const math::dcomplex & i1, const math::dcomplex & i2)
{
  mMatrix.coeffRef(row, col) += i1+i2;
}

math::vector IterativeSolver::solveSystem(const math::vector &rhs)
{
  math::vector efield;
  switch (mAlgorithm) {
  case BiCGStab:
    efield = bicgStab(rhs);
    break;
  case CGS:
    efield = cgs(rhs);
    break;
  }
  return efield;
}

namespace  {
  const bool r = Solver::factory().registerType(OBJECT_ID, boost::bind(boost::factory<IterativeSolver*>()));
}

}
}
