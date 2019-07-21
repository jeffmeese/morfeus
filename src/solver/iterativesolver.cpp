#include "iterativesolver.h"

#include "edge.h"
#include "element.h"
#include "mesh.h"
#include "meshinformation.h"

#include "xmlutils.h"

static const std::string OBJECT_ID("IterativeSolver");

IterativeSolver::IterativeSolver()
  : mAlgorithm(BiCG)
  , mMaxIterations(100)
  , mMinIterations(1)
  , mTolerance(0.001)
{
}

IterativeSolver::IterativeSolver(Algorithm algorithm, std::size_t minIterations, std::size_t maxIterations, double tolerance)
  : mAlgorithm(algorithm)
  , mMaxIterations(maxIterations)
  , mMinIterations(minIterations)
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

void IterativeSolver::doPrint(std::ostream & output, int tabPos) const
{
  std::string algoString = formatAlgoString(mAlgorithm);

  xmlutils::printHeader(output, tabPos, "Iterative Solver");
  xmlutils::printValue(output, tabPos+2, "Name: ", name());
  xmlutils::printValue(output, tabPos+2, "Number: ", number());
  xmlutils::printValue(output, tabPos+2, "Algorithm: ", algoString);
  xmlutils::printValue(output, tabPos+2, "Min Iterations: ", mMinIterations);
  xmlutils::printValue(output, tabPos+2, "Max Iterations: ", mMaxIterations);
  xmlutils::printValue(output, tabPos+2, "Tolerance: ", mTolerance);
}

void IterativeSolver::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  std::string algoString = xmlutils::readAttribute<std::string>(node, "algorithm");

  Algorithm algorithm = BiCG;
  if (algoString == "cgs")
    algorithm = CGS;
  else if (algoString == "bicg-stab")
    algorithm = BiCGStab;

  setName(xmlutils::readAttribute<std::string>(node, "name"));
  setNumber(std::stoi(xmlutils::readAttribute<std::string>(node, "number")));
  setAlgorithm(algorithm);
  setMinIterations(std::stoul(xmlutils::readAttribute<std::string>(node, "min-iterations")));
  setMaxIterations(std::stoul(xmlutils::readAttribute<std::string>(node, "max-iterations")));
  setTolerance(std::stod(xmlutils::readAttribute<std::string>(node, "tolerance")));
}

void IterativeSolver::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  std::string algoString = formatAlgoString(mAlgorithm);

  xmlutils::writeAttribute(document, node, "name", name());
  xmlutils::writeAttribute(document, node, "number", number());
  xmlutils::writeAttribute(document, node, "type", OBJECT_ID);
  xmlutils::writeAttribute(document, node, "algorithm", algoString);
  xmlutils::writeAttribute(document, node, "min-iterations", mMinIterations);
  xmlutils::writeAttribute(document, node, "max-iterations", mMaxIterations);
  xmlutils::writeAttribute(document, node, "tolerance", mTolerance);
}

std::string IterativeSolver::formatAlgoString(Algorithm algorithm) const
{
  if (algorithm == BiCG)
    return "bicg";
  else if (algorithm == CGS)
    return "cgs";
  else if (algorithm == BiCGStab)
    return "bicg-stab";

  return "Unknown";
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
