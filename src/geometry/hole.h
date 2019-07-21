#ifndef HOLE_H
#define HOLE_H

#include "morfeus.h"

#include "shape.h"

class Hole
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Hole();
  MORFEUS_LIB_DECL Hole(double x, double y);

public:
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL void setPosition(double x, double y);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  double mX;
  double mY;
};

#endif // HOLE_H
