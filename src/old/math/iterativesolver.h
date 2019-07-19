#ifndef MATH_SPARSESOLVER_H
#define MATH_SPARSESOLVER_H

#include "morfeus.h"

#include "math/matrixvectormultiply.h"

#include <complex>
#include <valarray>

namespace math
{
  class IterativeSolver
  {
    typedef std::complex<double> dcomplex;
    typedef std::valarray<dcomplex> dcomplex_array;

  public:
    MORFEUS_LIB_DECL IterativeSolver();
        
    MORFEUS_LIB_DECL bool converged() const;
    MORFEUS_LIB_DECL std::size_t getMinIterations() const;
    MORFEUS_LIB_DECL std::size_t getMaxIterations() const;
    MORFEUS_LIB_DECL double getTolerance() const;
    MORFEUS_LIB_DECL double getSolutionTolerance() const;
    MORFEUS_LIB_DECL void setMinIterations(std::size_t iter);
    MORFEUS_LIB_DECL void setMaxIterations(std::size_t iter);
    MORFEUS_LIB_DECL void setTolerance(double tolerance);
    MORFEUS_LIB_DECL void showStatus(bool show);

    MORFEUS_LIB_DECL dcomplex_array bicg(const dcomplex_array & rhs, const dcomplex_array & initial, const MatrixVectorMultiply<dcomplex> & mvm);
    MORFEUS_LIB_DECL dcomplex_array cgs(const dcomplex_array & rhs, const MatrixVectorMultiply<dcomplex> & mvm);
    MORFEUS_LIB_DECL dcomplex_array bicgstab(const dcomplex_array & rhs, const dcomplex_array & initial, const MatrixVectorMultiply<dcomplex> & mvm);

  private:
    bool mShowStatus;
    bool mConverged;
    std::size_t mMinIterations;
    std::size_t mMaxIterations;
    double mTolerance;
    double mSolutionTolerance;
  };
}

#endif // #ifndef MATH_SPARSESOLVER_H
