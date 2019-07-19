#ifndef ITERATIVESOLUTION_H
#define ITERATIVESOLUTION_H

#include "morfeus.h"
#include "types.h"

#include "solver/solution.h"

#include "math/uppertriangularmatrix.h"
#include "math/sparsematrix.h"
#include "math/matrixvectormultiply.h"

class IterativeSolution
    : public Solution
{
  friend class MatrixVectorMulitply;

public:
  MORFEUS_LIB_DECL IterativeSolution(const Mesh & mesh);

public:
  MORFEUS_LIB_DECL std::size_t maxIterations() const;
  MORFEUS_LIB_DECL std::size_t minIterations() const;
  MORFEUS_LIB_DECL double tolerance() const;
  MORFEUS_LIB_DECL void setMaxIterations(std::size_t iterations);
  MORFEUS_LIB_DECL void setMinIterations(std::size_t iterations);
  MORFEUS_LIB_DECL void setTolerance(double tolerance);

public:
  MORFEUS_LIB_DECL virtual bool buildMatrix(double freqGHz);
  MORFEUS_LIB_DECL virtual dcomplex_array solveSystem(const dcomplex_array & excitation);

private:
  void allocateMemory(const Mesh & mesh);
  void buildBiMatrix(double freq);
  void buildFeMatrix(double freq);

private:
  std::size_t mMinIterations;
  std::size_t mMaxIterations;
  double mTolerance;
  math::SparseMatrix<dcomplex> mMatrix;
  math::SparseMatrix<dcomplex> mPreconditioner;

private:
  class MatrixVectorMulitply
      : public math::MatrixVectorMultiply<dcomplex>
  {
    typedef math::SparseMatrix<dcomplex> Matrix;

  public:
    MatrixVectorMulitply(const IterativeSolution * solution);

    virtual dcomplex_array compute(const dcomplex_array & rhs) const;
    virtual dcomplex_array precondition(const dcomplex_array &rhs) const;

  private:
    const IterativeSolution * mSolution;
  };
};

#endif // ITERATIVESOLUTION_H
