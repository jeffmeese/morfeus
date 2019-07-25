#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"

class Cylinder
    : public Shape
{
public:
  MORFEUS_LIB_DECL Cylinder();
  MORFEUS_LIB_DECL Cylinder(const std::string & name);

public:
  MORFEUS_LIB_DECL double height() const;
  MORFEUS_LIB_DECL double radius() const;
  MORFEUS_LIB_DECL std::size_t resolution() const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL double z() const;
  MORFEUS_LIB_DECL void setHeight(double value);
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
  double mHeight;
  double mRadius;
  double mX;
  double mY;
  double mZ;
};

inline double Cylinder::height() const
{
  return mHeight;
}

inline double Cylinder::radius() const
{
  return mRadius;
}

inline std::size_t Cylinder::resolution() const
{
  return mResolution;
}

inline double Cylinder::x() const
{
  return mX;
}

inline double Cylinder::y() const
{
  return mY;
}

inline double Cylinder::z() const
{
  return mZ;
}

inline void Cylinder::setHeight(double value)
{
  mHeight = value;
}

inline void Cylinder::setRadius(double value)
{
  mRadius = value;
}

inline void Cylinder::setResolution(std::size_t value)
{
  mResolution = value;
}

inline void Cylinder::setX(double value)
{
  mX = value;
}

inline void Cylinder::setY(double value)
{
  mY = value;
}

inline void Cylinder::setZ(double value)
{
  mZ = value;
}

#endif // CYLINDER_H
