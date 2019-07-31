#ifndef MORFEUS_GEOMETRY_HOLE_H
#define MORFEUS_GEOMETRY_HOLE_H

#include "morfeus.h"

#include "core/morfeusobject.h"

#include "geometry/point.h"

namespace morfeus {
namespace geometry {

class Hole
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL Hole();
  MORFEUS_LIB_DECL Hole(const std::string & name);
  MORFEUS_LIB_DECL Hole(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Hole(const Point & pt);
  MORFEUS_LIB_DECL Hole(const std::string & name, const Point & pt);
  MORFEUS_LIB_DECL Hole(const std::string & id, const std::string & name, const Point & pt);

public:
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL Point position() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);
  MORFEUS_LIB_DECL void setPosition(const Point & pt);

protected:
  void doPrint(std::ostream & output, int tabPos = 0) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  std::string mName;
  Point mPosition;
};

inline std::string Hole::name() const
{
  return mName;
}

inline Point Hole::position() const
{
  return mPosition;
}

inline void Hole::setName(const std::string &name)
{
  mName = name;
}

inline void Hole::setPosition(const Point & pt)
{
  mPosition = pt;
}

}
}

#endif // MORFEUS_GEOMETRY_HOLE_H
