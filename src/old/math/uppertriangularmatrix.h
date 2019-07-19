#ifndef UPPER_TRIANGULAR_MATRIX_H
#define UPPER_TRIANGULAR_MATRIX_H

#include <map>
#include <valarray>
#include <cstddef>

namespace math
{
  template <typename T>
  class UpperTriangularMatrix
  {
  public:
    typedef typename std::pair<std::size_t, std::size_t> index;
    typedef typename std::pair<std::size_t, T> value;
    typedef T * iterator;

  public:
    UpperTriangularMatrix(std::size_t rows = 0)
    {
      resize(rows);
    }

    UpperTriangularMatrix(const UpperTriangularMatrix<T>& rhs)
    {
      *this = rhs;
    }

    UpperTriangularMatrix& operator=(const UpperTriangularMatrix<T>& rhs)
    {
      if (this == &rhs)
        return *this;

      resize(rhs.mRows);
      mData = rhs.mData;
      return *this;
    }
        
    virtual ~UpperTriangularMatrix()
    { }
        
    UpperTriangularMatrix& operator =(const T& val)
    {
      mData = val;
      return *this;
    }
        
    std::size_t full_size() const
    { return mRows * mRows; }
        
    std::size_t rows() const
    {
      return mRows;
    }

    std::size_t size() const
    { return mRows + mRows*(mRows-1)/2; }
        
    void resize(std::size_t rows)
    {
      mRows = rows;
      mData.resize(mRows + mRows*(mRows-1)/2);
    }
    
    void clear()
    {
      resize(0);
    }
        
    iterator begin() const
    { return &mData[0]; }
    
    iterator end() const
    { return &mData[size()-1]; }
    
    T operator()(std::size_t r, std::size_t c) const
    { return mData[get_index(r,c)]; }
        
    T& operator()(std::size_t r, std::size_t c)
    { return mData[get_index(r,c)]; }

    T operator[](std::size_t index) const
    { return mData[index]; }
    
    T& operator[](std::size_t index)
    { return mData[index]; }

        
    std::valarray<T> operator *(const std::valarray<T>& v) const
    {
      std::valarray<T> res(v.size());
      for (std::size_t i = 0; i < mRows; i++) {
        for (std::size_t j = i; j < mRows; j++) {
          T val = mData[get_index(i,j)];
          res[i] += val * v[j];
          if (i != j) res[j] += val * v[i];
        }
      }
      return res;
    }

  private:
    std::size_t get_index(std::size_t r, std::size_t c) const
    {
      if (r > c) {
        std::size_t tmp = c;
        c = r;
        r = tmp;
      }
      return (r+1) + (c * (c+1) / 2) - 1;
    }
        
  private:
    std::size_t mRows;
    std::valarray<T> mData;
  };
    
  template <typename T>
  inline std::ostream&
  operator <<(std::ostream& os, const UpperTriangularMatrix<T>& m)
  {
    typedef typename UpperTriangularMatrix<T>::iterator iterator;

    for (std::size_t i = 0; i < m.size(); i++) {
      os << i << " " << m[i] << "\n";
    }
    return os;
  }
}

#endif // #ifndef UPPER_TRIANGULAR_MATRIX_H
