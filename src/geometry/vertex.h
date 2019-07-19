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
  void doReadFromXml(ptree & tree) override;
  void doWriteToXml(ptree & tree) const override;

private:
  bool mBoundary;
  double mX;
  double mY;
  std::vector<double> mAttributes;
};

#endif // VERTEX_H
