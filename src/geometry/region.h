#ifndef REGION_H
#define REGION_H

#include "morfeus.h"
#include "morfeusobject.h"

class Region
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Region();
  MORFEUS_LIB_DECL Region(const std::string & name);
  MORFEUS_LIB_DECL Region(const std::string & id, const std::string & name);
  MORFEUS_LIB_DECL Region(double x, double y);
  MORFEUS_LIB_DECL Region(const std::string & name, double x, double y);
  MORFEUS_LIB_DECL Region(const std::string & id, const std::string & name, double x, double y);

public:
  MORFEUS_LIB_DECL double attribute() const;
  MORFEUS_LIB_DECL double maxArea() const;
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL void setAttribute(double value);
  MORFEUS_LIB_DECL void setMaxArea(double value);
  MORFEUS_LIB_DECL void setName(const std::string & name);
  MORFEUS_LIB_DECL void setPosition(double x, double y);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);

public:
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Region & object);

private:
  double mAttribute;
  double mMaxArea;
  double mX;
  double mY;
  std::string mName;
};

inline double Region::maxArea() const
{
  return mMaxArea;
}

inline std::string Region::name() const
{
  return mName;
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

inline void Region::setX(double value)
{
  mX = value;
}

inline void Region::setY(double value)
{
  mY = value;
}

inline double Region::x() const
{
  return mX;
}

inline double Region::y() const
{
  return mY;
}

#endif // REGION_H
