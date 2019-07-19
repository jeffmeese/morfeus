#ifndef MATH_MATRIXVECTORMULTIPLY_H
#define MATH_MATRIXVECTORMULTIPLY_H

#include <valarray>

namespace math
{
    template <typename T>
    class MatrixVectorMultiply
    {
    public:
      virtual std::valarray<T> precondition(const std::valarray<T> & rhs) const = 0;
      virtual std::valarray<T> compute(const std::valarray<T> & rhs) const = 0;
    };
    
}

#endif // #ifndef MATH_MATRIXVECTORMULTIPLY_H
