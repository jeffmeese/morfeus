#ifndef MORFEUS_MODEL_GEOMETRY_VERTEX_H
#define MORFEUS_MODEL_GEOMETRY_VERTEX_H

#include "core/morfeusobject.h"

namespace morfeus {
namespace model {
namespace geometry {

class Vertex
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL Vertex();
  MORFEUS_LIB_DECL Vertex(const std::string & name);
  MORFEUS_LIB_DECL Vertex(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Vertex(double x, double y, double z);
  MORFEUS_LIB_DECL Vertex(const std::string & name, double x, double y, double z);
  MORFEUS_LIB_DECL Vertex(const std::string & id, const std::string & name, double x, double y, double z);

public:
  MORFEUS_LIB_DECL bool boundary() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL double z() const;
  MORFEUS_LIB_DECL void setBoundary(bool value);
  MORFEUS_LIB_DECL void setPosition(double x, double y, double z);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);
  MORFEUS_LIB_DECL void setZ(double value);
  MORFEUS_LIB_DECL void setNumber(int32_t number) { mNumber = number;}

public:
  MORFEUS_LIB_DECL static int32_t totalCreated();

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  void init(double x, double y, double z);

private:
  static int32_t mTotal;
  bool mBoundary;
  int32_t mNumber;
  double mX;
  double mY;
  double mZ;
};

inline bool Vertex::boundary() const
{
  return mBoundary;
}

inline int32_t Vertex::number() const
{
  return mNumber;
}

inline void Vertex::setBoundary(bool value)
{
  mBoundary = value;
}

inline void Vertex::setX(double value)
{
  mX = value;
}

inline void Vertex::setY(double value)
{
  mY = value;
}

inline void Vertex::setZ(double value)
{
  mZ = value;
}

inline int32_t Vertex::totalCreated()
{
  return mTotal;
}

inline double Vertex::x() const
{
  return mX;
}

inline double Vertex::y() const
{
  return mY;
}

inline double Vertex::z() const
{
  return mZ;
}

}
}
}

#endif // MORFEUS_MODEL_GEOMETRY_VERTEX_H
