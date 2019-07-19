#ifndef VECTOR_H
#define VECTOR_H

namespace math
{
  template <typename T>
  class vector
  {
  public:
    vector()
      : mX(T())
      , mY(T())
      , mZ(T())
    {
    }

    vector(T x, T y, T z)
      : mX(x)
      , mY(y)
      , mZ(z)
    {
    }

    vector(const vector<T> & rhs)
      : mX(rhs.x())
      , mY(rhs.y())
      , mZ(rhs.z())
    {
    }

    vector & operator=(const vector<T> & rhs)
    {
      mX = rhs.x();
      mY = rhs.y();
      mZ = rhs.z();
    }

  public:
    T x() const
    { return mX; }

    T y() const
    { return mY; }

    T z() const
    { return mZ; }

    void setX(T x)
    { mX = x; }

    void setY(T y)
    { mY = y; }

    void setZ(T z)
    { mZ = z; }

    void set(T x, T y, T z)
    {
      mX = x;
      mY = y;
      mZ = z;
    }

  public:
    double length() const
    {
      return sqrt(mX*mX + mY*mY + mZ*mZ);
    }

    void move(T x, T y, T z)
    {
      set(x, y, z);
    }

    void normalize()
    {
      double denom = 1.0 / length();
      scale(denom, denom, denom);
    }

    void scale(T sx, T sy, T sz)
    {
      mX *= sx;
      mY *= sy;
      mZ *= sz;
    }

    void translate(T tx, T ty, T tz)
    {
      mX += tx;
      mY += ty;
      mZ += tz;
    }

		vector<T> & operator +=(const vector<T> & v)
		{
			mX += v.x();
			mY += v.y();
			mZ += v.z();
			return *this;
		}

		vector<T> & operator +=(const T & value)
		{
			mX += value;
			mY += value;
			mZ += value;
			return *this;
		}

		vector<T> & operator -=(const vector<T> & v)
		{
			mX -= v.x();
			mY -= v.y();
			mZ -= v.z();
			return *this;
		}

		vector<T> & operator -=(const T & value)
		{
			mX -= value;
			mY -= value;
			mZ -= value;
			return *this;
		}

		vector<T> & operator *=(const T & value)
		{
			mX *= value;
			mY *= value;
			mZ *= value;
			return *this;
		}

		vector<T> & operator /=(const T & value)
		{
			mX /= value;
			mY /= value;
			mZ /= value;
			return *this;
		}

  private:
    T mX;
    T mY;
    T mZ;
  };

  template <typename T>
  math::vector<T> operator +(const math::vector<T>& v1, const math::vector<T>& v2)
  {
    return math::vector<T>(v1.x()+v2.x(), v1.y()+v2.y(), v1.z()+v2.z());
  }

  template <typename T>
  math::vector<T> operator +(const math::vector<T> & v1, const T & value)
  {
    return math::vector<T>(v1.x()+value, v1.y()+value, v1.z()+value);
  }

  template <typename T>
  math::vector<T> operator -(const math::vector<T>& v1)
  {
    return math::vector<T>(-v1.x(), -v1.y(), -v1.z());
  }

  template <typename T>
  math::vector<T> operator -(const math::vector<T>& v1, const math::vector<T>& v2)
  {
    return math::vector<T>(v1.x()-v2.x(), v1.y()-v2.y(), v1.z()-v2.z());
  }

  template <typename T>
  math::vector<T> operator -(const math::vector<T> & v1, const T & value)
  {
    return math::vector<T>(v1.x()-value, v1.y()-value, v1.z()-value);
  }

  template <typename T>
  math::vector<T> operator *(const math::vector<T> & v1, const T & value)
  {
    return math::vector<T>(v1.x()*value, v1.y()*value, v1.z()*value);
  }

  template <typename T>
  math::vector<T> operator /(const math::vector<T> & v1, const T & value)
  {
    return math::vector<T>(v1.x()/value, v1.y()/value, v1.z()/value);
  }

  template <typename T>
  bool operator ==(const math::vector<T> & v1, const math::vector<T> & v2)
  {
    return ( (v1.x() == v2.x())&& (v1.y() == v2.y())&& (v1.z() == v2.z()) );
  }

  template <typename T>
  bool operator !=(const math::vector<T> & v1, const math::vector<T> & v2)
  {
    return !(v1 == v2);
  }
}

#endif // VECTOR_H
