#ifndef SOLVER_H
#define SOLVER_H

#include "morfeus.h"

#include <boost/noncopyable.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/vector.hpp>

class Excitation;
class Mesh;
class MeshInformation;
class Observation;
class Solution;

class Solver
    : private boost::noncopyable
{
public:
  MORFEUS_LIB_DECL Solver();
  MORFEUS_LIB_DECL virtual ~Solver();

public:
  MORFEUS_LIB_DECL void runSolver(double freqGhz, const Mesh * mesh, Solution * solution);

protected:
  typedef boost::numeric::ublas::mapped_matrix<dcomplex> sparse_matrix;
  typedef boost::numeric::ublas::triangular_matrix<dcomplex> triangular_matrix;
  typedef boost::numeric::ublas::vector<dcomplex> vector;

protected:
  virtual void allocateMatrices(const MeshInformation * meshInfo) = 0;
  virtual void clearMatrices(const Mesh * mesh, const MeshInformation * meshInfo) = 0;
  virtual vector solveSystem(const vector & rhs) = 0;
  virtual void updateBoundaryIntegralMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2) = 0;
  virtual void updateFiniteElementMatrix(std::size_t row, std::size_t col, const dcomplex & i1, const dcomplex & i2) = 0;

private:
  void buildBiMatrix(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo);
  void buildFeMatrix(double freqGHz, const Mesh * mesh, const MeshInformation * meshInfo);

private:
  typedef std::unique_ptr<MeshInformation> MeshInformationPtr;

private:
  bool mAllocated;
  vector mEfield;
  vector mRhs;
  MeshInformationPtr mMeshInfo;
};

#endif // SOLVER_H
