#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <map>
#include <vector>
#include <algorithm>
#include <valarray>
#include <iostream>
#include <cstddef>

namespace math
{
  template <typename T>
  class SparseMatrix
  {
  public:
    typedef typename std::pair<std::size_t, std::size_t> index;
    typedef T value;
    typedef typename std::map<index,value>::iterator iterator;
    typedef typename std::map<index,value>::const_iterator const_iterator;

  public:
    SparseMatrix()
    { }

    SparseMatrix(const SparseMatrix<T>& rhs)
    {
      *this = rhs;
    }

    SparseMatrix& operator=(const SparseMatrix<T>& rhs)
    {
      if (this == &rhs) return *this;
      mData = rhs.mData;
      return *this;
    }

    SparseMatrix& operator =(const T& val)
    {
      for (iterator itr = mData.begin(); itr != mData.end(); ++itr)
        itr->second = val;
      return *this;
    }

    virtual ~SparseMatrix()
    { }

    std::size_t size() const
    {
      return mData.size();
    }

    bool exists(std::size_t r, std::size_t c) const
    {
      return mData.find(index(r,c)) != mData.end();
    }

    void insert(std::size_t r, std::size_t c, T val)
    {
      mData.insert(std::make_pair(index(r,c), val));
    }

    T value_at(std::size_t r, std::size_t c) const
    {
      const_iterator itr = mData.find(index(r,c));
      return (itr == mData.end()) ? T() : itr->second;
    }
            

    T& value_at(std::size_t r, std::size_t c)
    {
      return mData[index(r,c)];
    }

    T operator()(std::size_t r, std::size_t c) const
    {
      return value_at(r,c);
    }

    T& operator()(std::size_t r, std::size_t c)
    {
      return value_at(r,c);
    }

    SparseMatrix<T> operator +(const SparseMatrix<T> & rhs) const
    {
      SparseMatrix<T> m(*this);
      for (const_iterator itr = rhs.mData.begin(); itr != rhs.mData.end(); ++itr) {
        int r = itr->first.first;
        int c = itr->first.second;
        m.mData[index(r,c)] += rhs(r,c);
      }
      return m;
    }
        
    SparseMatrix<T> operator -(const SparseMatrix<T> & rhs) const
    {
      SparseMatrix<T> m(*this);
      for (const_iterator itr = rhs.mData.begin(); itr != rhs.mData.end(); ++itr) {
        int r = itr->first.first;
        int c = itr->first.second;
        m.mData[index(r,c)] -= rhs(r,c);
      }
      return m;
    }
                
    SparseMatrix<T> operator*(const T val)
    {
      SparseMatrix<T> m(*this);
      for (iterator itr = mData.begin(); itr != mData.end(); ++itr) {
        int r = itr->first.first;
        int c = itr->first.second;
        index i = std::make_pair(r,c);
        m.mData[i] *= val;
      }
      return m;
    }
        
    // Matrix-Vector-Multiply with valarrays
    std::valarray<T> operator*(const std::valarray<T>& v) const
    {
      std::valarray<T> res(v.size());
      for (const_iterator itr = mData.begin(); itr != mData.end(); ++itr)
        res[itr->first.first] += itr->second * v[itr->first.second];
      return res;
    }

    	
    // Matrix-Vector-Multiply with raw arrays
    void mvm(const T * in, T * out)
    {
      for (const_iterator itr = mData.begin(); itr != mData.end(); ++itr)
        out[itr->first.first] += itr->second * in[itr->first.second];
    }
        
    void clear()
    {
      mData.clear();
    }

    iterator begin()
    {
      return mData.begin();
    }

    iterator end()
    {
      return mData.end();
    }

    const_iterator begin() const
    {
      return mData.begin();
    }

    const_iterator end() const
    {
      return mData.end();
    }
    
  private:
    std::map<index,value> mData;  // Maps (row,col)->value
  };
    
  template <typename T>
  inline std::ostream&
  operator <<(std::ostream& os, const SparseMatrix<T>& m)
  {
    typedef typename SparseMatrix<T>::const_iterator const_iterator;
    for (const_iterator itr = m.begin(); itr != m.end(); ++itr) {
      os << itr->first.first << " " << itr->first.second << " " << itr->second << "\n";
    }
    return os;
  }
}
    
#endif // #ifndef SPARSEMATRIX_H
