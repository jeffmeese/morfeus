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
  MORFEUS_LIB_DECL Hole(double x, double y, double z);
  MORFEUS_LIB_DECL Hole(const std::string & name, double x, double y, double z);
  MORFEUS_LIB_DECL Hole(const std::string & id, const std::string & name, double x, double y, double z);

public:
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL double z() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);
  MORFEUS_LIB_DECL void setPosition(double x, double y, double z);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);
  MORFEUS_LIB_DECL void setZ(double value);

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
  double mZ;
};

inline std::string Hole::name() const
{
  return mName;
}

inline void Hole::setName(const std::string &name)
{
  mName = name;
}

inline void Hole::setX(double value)
{
  mX = value;
}

inline void Hole::setY(double value)
{
  mY = value;
}

inline void Hole::setZ(double value)
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

inline double Hole::z() const
{
  return mZ;
}

#endif // HOLE_H
