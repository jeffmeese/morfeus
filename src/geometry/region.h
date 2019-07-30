#ifndef REGION_H
#define REGION_H

#include "morfeus.h"
#include "morfeusobject.h"
#include "point3d.h"
#include "rapidxml.hpp"
#include "xmlutils.h"

namespace morfeus {
namespace geometry {

class Region
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Region();
  MORFEUS_LIB_DECL Region(const std::string & name);
  MORFEUS_LIB_DECL Region(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Region(const Point3D & pt);
  MORFEUS_LIB_DECL Region(const std::string & name, const Point3D & pt);
  MORFEUS_LIB_DECL Region(const std::string & id, const std::string & name, const Point3D & pt);

public:
  MORFEUS_LIB_DECL double attribute() const;
  MORFEUS_LIB_DECL double maxArea() const;
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL Point3D position() const;
  MORFEUS_LIB_DECL void setAttribute(double value);
  MORFEUS_LIB_DECL void setMaxArea(double value);
  MORFEUS_LIB_DECL void setName(const std::string & name);
  MORFEUS_LIB_DECL void setPosition(const Point3D & pt);

public:
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Region & object);

private:
  double mAttribute;
  double mMaxArea;
  std::string mName;
  Point3D mPosition;
};

inline double Region::maxArea() const
{
  return mMaxArea;
}

inline std::string Region::name() const
{
  return mName;
}

inline Point3D Region::position() const
{
  return mPosition;
}

inline void Region::setAttribute(double value)
{
  mAttribute = value;
}

inline void Region::setMaxArea(double value)
{
  mMaxArea = value;
}

inline void Region::setName(const std::string &name)
{
  mName = name;
}

inline void Region::setPosition(const Point3D & pt)
{
  mPosition = pt;
}

}
}

#endif // REGION_H
