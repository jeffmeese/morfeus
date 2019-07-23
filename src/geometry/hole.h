#ifndef HOLE_H
#define HOLE_H

#include "morfeus.h"

#include "shape.h"

class Hole
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Hole();
  MORFEUS_LIB_DECL Hole(const std::string & name);
  MORFEUS_LIB_DECL Hole(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Hole(double x, double y);
  MORFEUS_LIB_DECL Hole(const std::string & name, double x, double y);
  MORFEUS_LIB_DECL Hole(const std::string & id, const std::string & name, double x, double y);

public:
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL void setPosition(double x, double y);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);

public:
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Hole & object);

private:
  std::string mName;
  double mX;
  double mY;
};

inline void Hole::setX(double value)
{
  mX = value;
}

inline void Hole::setY(double value)
{
  mY = value;
}

inline double Hole::x() const
{
  return mX;
}

inline double Hole::y() const
{
  return mY;
}

#endif // HOLE_H
