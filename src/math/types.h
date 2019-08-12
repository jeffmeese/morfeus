#ifndef TYPES_H
#define TYPES_H

#include <complex>

#include <boost/qvm/vec.hpp>

#include "Eigen/Core"
#include "Eigen/SparseCore"

namespace morfeus {
namespace math {

  typedef std::complex<double> dcomplex;
  typedef std::complex<float> fcomplex;
  typedef boost::qvm::vec<double, 3> vec3d;
  typedef Eigen::VectorXcd vector;
  typedef Eigen::SparseMatrix<dcomplex> sparse_matrix;
}
}

#endif // TYPES_H
