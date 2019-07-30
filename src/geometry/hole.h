#ifndef HOLE_H
#define HOLE_H

#include "morfeus.h"
#include "point3d.h"
#include "shape.h"

namespace Morfeus {
namespace Geometry {

class Hole
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Hole();
  MORFEUS_LIB_DECL Hole(const std::string & name);
  MORFEUS_LIB_DECL Hole(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Hole(const Point3D & pt);
  MORFEUS_LIB_DECL Hole(const std::string & name, const Point3D & pt);
  MORFEUS_LIB_DECL Hole(const std::string & id, const std::string & name, const Point3D & pt);

public:
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL Point3D position() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);
  MORFEUS_LIB_DECL void setPosition(const Point3D & pt);
public:
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Hole & object);

private:
  std::string mName;
  Point3D mPosition;
};

inline std::string Hole::name() const
{
  return mName;
}

inline Point3D Hole::position() const
{
  return mPosition;
}

inline void Hole::setName(const std::string &name)
{
  mName = name;
}

inline void Hole::setPosition(const Point3D & pt)
{
  mPosition = pt;
}

}
}

#endif // HOLE_H
