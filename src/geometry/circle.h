#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"
#include "point2d.h"

class Circle
    : public Shape
{
public:
  MORFEUS_LIB_DECL Circle();
  MORFEUS_LIB_DECL Circle(const std::string & name);
  MORFEUS_LIB_DECL Circle(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Circle(double x, double y, double z, double radius);
  MORFEUS_LIB_DECL Circle(const std::string & name, double x, double y, double z, double radius);
  MORFEUS_LIB_DECL Circle(const std::string & id, const std::string & name, double x, double y, double z, double radius);

public:
  MORFEUS_LIB_DECL double radius() const;
  MORFEUS_LIB_DECL std::size_t resolution() const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL double z() const;
  MORFEUS_LIB_DECL void setCenter(double x, double y, double z);
  MORFEUS_LIB_DECL void setRadius(double value);
  MORFEUS_LIB_DECL void setResolution(std::size_t value);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);
  MORFEUS_LIB_DECL void setZ(double value);

protected:
  std::vector<MesherPolygon> doGetMesherPolygons() const override;
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  std::size_t mResolution;
  double mX;
  double mY;
  double mZ;
  double mRadius;
};

inline double Circle::radius() const
{
  return mRadius;
}

inline std::size_t Circle::resolution() const
{
  return mResolution;
}

inline double Circle::x() const
{
  return mX;
}

inline double Circle::y() const
{
  return mY;
}

inline double Circle::z() const
{
  return mZ;
}

inline void Circle::setRadius(double value)
{
  mRadius =  value;
}

inline void Circle::setResolution(std::size_t value)
{
  mResolution = value;
}

inline void Circle::setX(double value)
{
  mX = value;
}

inline void Circle::setY(double value)
{
  mY = value;
}

inline void Circle::setZ(double value)
{
  mZ = value;
}

#endif // CIRCLE_H
