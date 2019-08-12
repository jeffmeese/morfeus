#ifndef MORFEUS_MODEL_GEOMETRY_HOLE_H
#define MORFEUS_MODEL_GEOMETRY_HOLE_H

#include "core/morfeusobject.h"

#include "point.h"

namespace morfeus {
namespace model {
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
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL Point position() const;
  MORFEUS_LIB_DECL void setPosition(const Point & pt);

public:
  MORFEUS_LIB_DECL static int32_t totalCreated();

protected:
  void doPrint(std::ostream & output, int tabPos = 0) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void init(const Point & pt);

private:
  static int32_t mTotal;
  int32_t mNumber;
  Point mPosition;
};

inline int32_t Hole::number() const
{
  return mNumber;
}

inline Point Hole::position() const
{
  return mPosition;
}

inline void Hole::setPosition(const Point & pt)
{
  mPosition = pt;
}

inline int32_t Hole::totalCreated()
{
  return mTotal;
}

}
}
}

#endif // MORFEUS_MODEL_GEOMETRY_HOLE_H
