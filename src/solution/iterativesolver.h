#ifndef ITERATIVESOLVER_H
#define ITERATIVESOLVER_H

#include "solver.h"

class IterativeSolver
    : public Solver
{
public:
  enum Algorithm
  {
    BiCG = 0,
    CGS = 1,
    BiCGStab = 2
  };

public:
  MORFEUS_LIB_DECL IterativeSolver();
  MORFEUS_LIB_DECL IterativeSolver(Algorithm algorithm, std::size_t minIterations, std::size_t maxIterations, double tolerance);

public:
  MORFEUS_LIB_DECL Algorithm algorithm() const;
  MORFEUS_LIB_DECL std::size_t maxIterations() const;
  MORFEUS_LIB_DECL std::size_t minIterations() const;
  MORFEUS_LIB_DECL double tolerance() const;
  MORFEUS_LIB_DECL void setAlgorithm(Algorithm value);
  MORFEUS_LIB_DECL void setMaxIterations(std::size_t value);
  MORFEUS_LIB_DECL void setMinIterations(std::size_t value);
  MORFEUS_LIB_DECL void setTolerance(double value);

protected:
  void allocateMatrices(const MeshInformation * meshInfo) override;
  void clearMatrices(const Mesh * mesh, const MeshInformation * meshInfo) override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;
  void updateBoundaryIntegralMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2) override;
  void updateFiniteElementMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2) override;
  vector solveSystem(const vector & rhs) override;

private:
  vector bicg(const vector & rhs);
  vector bicgStab(const vector & rhs);
  vector cgs(const vector & rhs);
  Algorithm readAlgorithmAttribute(rapidxml::xml_node<> * node) const;
  std::string formatAlgoString(Algorithm algorithm) const;

private:
  Algorithm mAlgorithm;
  triangular_matrix mBiMatrix;
  sparse_matrix mFeMatrix;
  std::size_t mMaxIterations;
  std::size_t mMinIterations;
  double mTolerance;
};

inline IterativeSolver::Algorithm IterativeSolver::algorithm() const
{
  return mAlgorithm;
}

inline std::size_t IterativeSolver::maxIterations() const
{
  return mMaxIterations;
}

inline std::size_t IterativeSolver::minIterations() const
{
  return mMinIterations;
}

inline double IterativeSolver::tolerance() const
{
  return mTolerance;
}

inline void IterativeSolver::setAlgorithm(Algorithm value)
{
  mAlgorithm = value;
}

inline void IterativeSolver::setMaxIterations(std::size_t value)
{
  mMaxIterations = value;
}

inline void IterativeSolver::setMinIterations(std::size_t value)
{
  mMinIterations = value;
}

inline void IterativeSolver::setTolerance(double value)
{
  mTolerance = value;
}

#endif // ITERATIVESOLVER_H
