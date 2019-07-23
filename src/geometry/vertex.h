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
  MORFEUS_LIB_DECL Vertex(const std::string & id);
  MORFEUS_LIB_DECL Vertex(int32_t number);
  MORFEUS_LIB_DECL Vertex(const std::string & id, int32_t number);
  MORFEUS_LIB_DECL Vertex(double x, double y);
  MORFEUS_LIB_DECL Vertex(const std::string & id, double x, double y);
  MORFEUS_LIB_DECL Vertex(int32_t number, double x, double y);
  MORFEUS_LIB_DECL Vertex(const std::string & id, int32_t number, double x, double y);

public:
  MORFEUS_LIB_DECL bool boundary() const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL void setBoundary(bool value);
  MORFEUS_LIB_DECL void setNumber(int32_t value);
  MORFEUS_LIB_DECL void setPosition(double x, double y);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);

public:
  MORFEUS_LIB_DECL void addAttribute(double value);

public:
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Vertex & object);

private:
  bool mBoundary;
  int32_t mNumber;
  double mX;
  double mY;
  std::vector<double> mAttributes;
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

inline void Vertex::setNumber(int32_t value)
{
  mNumber = value;
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
