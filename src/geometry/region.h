#ifndef REGION_H
#define REGION_H

#include "morfeus.h"
#include "morfeusobject.h"

class Region
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Region();
  MORFEUS_LIB_DECL Region(double x, double y);

public:
  MORFEUS_LIB_DECL double attribute() const;
  MORFEUS_LIB_DECL double maxArea() const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL void setAttribute(double value);
  MORFEUS_LIB_DECL void setMaxArea(double value);
  MORFEUS_LIB_DECL void setPosition(double x, double y);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  double mAttribute;
  double mMaxArea;
  double mX;
  double mY;
};

#endif // REGION_H
