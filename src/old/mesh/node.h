#ifndef NODE_H
#define NODE_H

#include "morfeus.h"

#include <cstdlib>

class Node
{
public:
  MORFEUS_LIB_DECL Node(std::size_t id);
  MORFEUS_LIB_DECL Node(std::size_t id, double x, double y, double z);

public:
  MORFEUS_LIB_DECL std::size_t id() const;
  MORFEUS_LIB_DECL double x() const;
  MORFEUS_LIB_DECL double y() const;
  MORFEUS_LIB_DECL double z() const;
  MORFEUS_LIB_DECL void setX(double x);
  MORFEUS_LIB_DECL void setY(double y);
  MORFEUS_LIB_DECL void setZ(double z);

public:
  MORFEUS_LIB_DECL void move(double x, double y, double z);
  MORFEUS_LIB_DECL void scale(double sx, double sy, double sz);
  MORFEUS_LIB_DECL void translate(double dx, double dy, double dz);

private:
  std::size_t mId;
  double mX;
  double mY;
  double mZ;
};

#endif // NODE_H
