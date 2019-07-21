#ifndef VERTEX_H
#define VERTEX_H

#include "morfeus.h"
#include "morfeusobject.h"

#include <vector>

class Vertex
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Vertex();
  MORFEUS_LIB_DECL Vertex(double x, double y);
  MORFEUS_LIB_DECL Vertex(int32_t number, double x, double y);

public:
  MORFEUS_LIB_DECL bool boundary() const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL void setBoundary(bool value);
  MORFEUS_LIB_DECL void setPosition(double x, double y);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);

public:
  MORFEUS_LIB_DECL void addAttribute(double value);

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  bool mBoundary;
  double mX;
  double mY;
  std::vector<double> mAttributes;
};

inline bool Vertex::boundary() const
{
  return mBoundary;
}

inline void Vertex::setBoundary(bool value)
{
  mBoundary = value;
}

inline void Vertex::setPosition(double x, double y)
{
  setX(x);
  setY(y);
}

inline void Vertex::setX(double value)
{
  mX = value;
}

inline void Vertex::setY(double value)
{
  mY = value;
}

inline double Vertex::x() const
{
  return mX;
}

inline double Vertex::y() const
{
  return mY;
}

#endif // VERTEX_H
