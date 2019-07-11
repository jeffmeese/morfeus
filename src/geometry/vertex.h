#ifndef VERTEX_H
#define VERTEX_H

#include "morfeus.h"
#include "types.h"

class Vertex
{
public:
  MORFEUS_LIB_DECL Vertex(int32_t id);
  MORFEUS_LIB_DECL Vertex(int32_t id, double x, double y);

public:
  MORFEUS_LIB_DECL bool boundary() const;
  MORFEUS_LIB_DECL int32_t id() const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL void setBoundary(bool value);
  MORFEUS_LIB_DECL void setPosition(double x, double y);
  MORFEUS_LIB_DECL void setX(double value);
  MORFEUS_LIB_DECL void setY(double value);

public:
  MORFEUS_LIB_DECL void addAttribute(double value);

private:
  bool mBoundary;
  int32_t mId;
  double mX;
  double mY;
  std::vector<double> mAttributes;
};

#endif // VERTEX_H
