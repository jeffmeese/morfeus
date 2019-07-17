#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include "morfeus.h"

#include <cstddef>

template <typename T>
class Matrix3x3
{
  // Constructors
public:
  Matrix3x3();
  Matrix3x3(const T & value);
  Matrix3x3(const Matrix3x3 & rhs);
  Matrix3x3 & operator=(const Matrix3x3 & rhs);

  // Properties
public:
  T value(std::size_t row, std::size_t col) const;
  T & value(std::size_t row, std::size_t col);
  void set(const T & value);
  void setValue(std::size_t row, std::size_t col, const T & value);

  // Methods
public:
  void identity();
  Matrix3x3 inverse() const;

  // Access overloads
public:
  T operator()(std::size_t row, std::size_t col) const;
  T & operator()(std::size_t row, std::size_t col);

  // Arithmetic overloads
public:
  Matrix3x3 & operator+=(const T & value);
  Matrix3x3 & operator-=(const T & value);
  Matrix3x3 & operator*=(const T & value);
  Matrix3x3 & operator/=(const T & value);
  Matrix3x3 & operator+=(const Matrix3x3<T> & m2);
  Matrix3x3 & operator-=(const Matrix3x3<T> & m2);
  Matrix3x3 & operator*=(const Matrix3x3<T> & m2);
  Matrix3x3 operator+(const T & value);
  Matrix3x3 operator-(const T & value);
  Matrix3x3 operator*(const T & value);
  Matrix3x3 operator/(const T & value);
  Matrix3x3 operator+(const Matrix3x3<T> & m2);
  Matrix3x3 operator-(const Matrix3x3<T> & m2);
  Matrix3x3 operator*(const Matrix3x3<T> & m2);

  // Global operator overloads
public:
  friend Matrix3x3 operator+(const Matrix3x3<T> & m1, const Matrix3x3<T> & m2);
  friend Matrix3x3 operator-(const Matrix3x3<T> & m1, const Matrix3x3<T> & m2);
  friend Matrix3x3 operator*(const Matrix3x3<T> & m1, const Matrix3x3<T> & m2);

private:
  T mData[3][3];
};

template <typename T>
Matrix3x3<T>::Matrix3x3()
{
  set(T());
}

template <typename T>
Matrix3x3<T>::Matrix3x3(const T & value)
{
  set(value);
}

template <typename T>
Matrix3x3<T>::Matrix3x3(const Matrix3x3<T> & rhs)
{
  *this = rhs;
}

template <typename T>
Matrix3x3<T> & Matrix3x3<T>::operator=(const Matrix3x3<T> & rhs)
{
  for (std::size_t i = 0; i < 3; i++) {
    for (std::size_t j = 0; j < 3; j++) {
      mData[i][j] = rhs.mData[i][j];
    }
  }
  return *this;
}

template <typename T>
Matrix3x3<T> & Matrix3x3<T>::operator+=(const T & value)
{
  for (std::size_t i = 0; i < 3; i++) {
    for (std::size_t j = 0; j < 3; j++) {
      mData[i][j] += value;
    }
  }
  return *this;
}

template <typename T>
Matrix3x3<T> & Matrix3x3<T>::operator-=(const T & value)
{
  for (std::size_t i = 0; i < 3; i++) {
    for (std::size_t j = 0; j < 3; j++) {
      mData[i][j] -= value;
    }
  }
  return *this;
}

template <typename T>
Matrix3x3<T> & Matrix3x3<T>::operator*=(const T & value)
{
  for (std::size_t i = 0; i < 3; i++) {
    for (std::size_t j = 0; j < 3; j++) {
      mData[i][j] *= value;
    }
  }
  return *this;
}

template <typename T>
Matrix3x3<T> & Matrix3x3<T>::operator/=(const T & value)
{
  for (std::size_t i = 0; i < 3; i++) {
    for (std::size_t j = 0; j < 3; j++) {
      mData[i][j] /= value;
    }
  }
  return *this;
}

template <typename T>
Matrix3x3<T> & Matrix3x3<T>::operator+=(const Matrix3x3<T> & m2)
{
  for (std::size_t i = 0; i < 3; i++) {
    for (std::size_t j = 0; j < 3; j++) {
      mData[i][j] += m2(i,j);
    }
  }
  return *this;
}

template <typename T>
Matrix3x3<T> & Matrix3x3<T>::operator-=(const Matrix3x3<T> & m2)
{
  for (std::size_t i = 0; i < 3; i++) {
    for (std::size_t j = 0; j < 3; j++) {
      mData[i][j] -= m2(i,j);
    }
  }
  return *this;
}

template <typename T>
Matrix3x3<T> & Matrix3x3<T>::operator*=(const Matrix3x3<T> & m2)
{

}

template <typename T>
Matrix3x3<T> Matrix3x3<T>::operator+(const T & value)
{
  Matrix3x3<T> m(*this);
  m += value;
  return m;
}

template <typename T>
Matrix3x3<T> Matrix3x3<T>::operator-(const T & value)
{
  Matrix3x3<T> m(*this);
  m -= value;
  return m;
}

template <typename T>
Matrix3x3<T> Matrix3x3<T>::operator*(const T & value)
{
  Matrix3x3<T> m(*this);
  m *= value;
  return m;
}

template <typename T>
Matrix3x3<T> Matrix3x3<T>::operator/(const T & value)
{
  Matrix3x3<T> m(*this);
  m /= value;
  return m;
}

template <typename T>
Matrix3x3<T> Matrix3x3<T>::operator+(const Matrix3x3<T> & m2)
{
  Matrix3x3<T> m(*this);
  m += m2;
  return m;
}

template <typename T>
Matrix3x3<T> Matrix3x3<T>::operator-(const Matrix3x3<T> & m2)
{
  Matrix3x3<T> m(*this);
  m -= m2;
  return m;
}

template <typename T>
Matrix3x3<T> Matrix3x3<T>::operator*(const Matrix3x3<T> & m2)
{
  (*this)(0,0) = (*this)(0,0)*m2(0,0) + m1(0,1)*m2(1,0) + (*this)(0,2)*m2(2,0);
  (*this)(0,1) = (*this)(0,0)*m2(0,1) + (*this)(0,1)*m2(1,1) + (*this)(0,2)*m2(2,1);
  (*this)(0,2) = (*this)(0,0)*m2(0,2) + (*this)(0,1)*m2(1,2) + (*this)(0,2)*m2(2,2);
  (*this)(1,0) = (*this)(1,0)*m2(0,0) + (*this)(1,1)*m2(1,0) + (*this)(1,2)*m2(2,0);
  (*this)(1,1) = (*this)(1,0)*m2(0,1) + (*this)(1,1)*m2(1,1) + (*this)(1,2)*m2(2,1);
  (*this)(2,2) = (*this)(1,0)*m2(0,2) + (*this)(1,1)*m2(1,2) + (*this)(1,2)*m2(2,2);
  (*this)(2,0) = (*this)(2,0)*m2(0,0) + (*this)(2,1)*m2(1,0) + (*this)(2,2)*m2(2,0);
  (*this)(2,1) = (*this)(2,0)*m2(0,1) + (*this)(2,1)*m2(1,1) + (*this)(2,2)*m2(2,1);
  (*this)(2,2) = (*this)(2,0)*m2(0,2) + (*this)(2,1)*m2(1,2) + (*this)(2,2)*m2(2,2);

  return *this;
}

template <typename T>
T Matrix3x3<T>::operator()(std::size_t row, std::size_t col) const
{
  return value(row, col);
}

template <typename T>
T & Matrix3x3<T>::operator()(std::size_t row, std::size_t col)
{
  return value(row, col);
}

template <typename T>
void Matrix3x3<T>::identity()
{
  setValue(T(1));
}

template <typename T>
Matrix3x3<T> Matrix3x3<T>::inverse() const
{
  Matrix3x3<T> m;
  return m;
}

template <typename T>
T Matrix3x3<T>::value(std::size_t row, std::size_t col) const
{
  return mData[row][col];
}

template <typename T>
T & Matrix3x3<T>::value(std::size_t row, std::size_t col)
{
  return mData[row][col];
}

template <typename T>
void Matrix3x3<T>::set(const T & value)
{
  for (std::size_t i = 0; i < 3; i++) {
    for (std::size_t j = 0; j < 3; j++) {
      mData[i][j] = value;
    }
  }
}

template <typename T>
void Matrix3x3<T>::setValue(std::size_t row, std::size_t col, const T & value)
{
  mData[row][col] = value;
}

template <typename T>
Matrix3x3<T> operator+(const Matrix3x3<T> & m1, const Matrix3x3<T> & m2)
{
  Matrix3x3<T> m;
  for (std::size_t i = 0; i < 3; i++) {
    for (std::size_t j = 0; j < 3; j++) {
      m(i,j) = m1(i,j) + m2(i,j);
    }
  }
}

template <typename T>
Matrix3x3<T> operator-(const Matrix3x3<T> & m1, const Matrix3x3<T> & m2)
{
  Matrix3x3<T> m;
  for (std::size_t i = 0; i < 3; i++) {
    for (std::size_t j = 0; j < 3; j++) {
      m(i,j) = m1(i,j) - m2(i,j);
    }
  }
}

template <typename T>
Matrix3x3<T> operator*(const Matrix3x3<T> & m1, const Matrix3x3<T> & m2)
{
  Matrix3x3<T> m;

  m(0,0) = m1(0,0)*m2(0,0) + m1(0,1)*m2(1,0) + m1(0,2)*m2(2,0);
  m(0,1) = m1(0,0)*m2(0,1) + m1(0,1)*m2(1,1) + m1(0,2)*m2(2,1);
  m(0,2) = m1(0,0)*m2(0,2) + m1(0,1)*m2(1,2) + m1(0,2)*m2(2,2);
  m(1,0) = m1(1,0)*m2(0,0) + m1(1,1)*m2(1,0) + m1(1,2)*m2(2,0);
  m(1,1) = m1(1,0)*m2(0,1) + m1(1,1)*m2(1,1) + m1(1,2)*m2(2,1);
  m(2,2) = m1(1,0)*m2(0,2) + m1(1,1)*m2(1,2) + m1(1,2)*m2(2,2);
  m(2,0) = m1(2,0)*m2(0,0) + m1(2,1)*m2(1,0) + m1(2,2)*m2(2,0);
  m(2,1) = m1(2,0)*m2(0,1) + m1(2,1)*m2(1,1) + m1(2,2)*m2(2,1);
  m(2,2) = m1(2,0)*m2(0,2) + m1(2,1)*m2(1,2) + m1(2,2)*m2(2,2);

  return m;
}

#endif // MATRIX3X3_H
