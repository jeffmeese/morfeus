#ifndef EDGE_H
#define EDGE_H

#include "morfeus.h"

#include <cstdlib>

class Mesh;

class Edge
{
public:
  enum class Type
  {
    Unknown = -1,
    Dielectric = 0,
    Boundary = 1,
    Metal = 2,
    Antenna = 3
  };

public:
  MORFEUS_LIB_DECL Edge(std::size_t id);
  MORFEUS_LIB_DECL Edge(std::size_t id, std::size_t globalNode1, std::size_t globalNode2);

public:
  MORFEUS_LIB_DECL double computeLength(const Mesh & mesh) const;
  MORFEUS_LIB_DECL std::size_t globalNode1() const;
  MORFEUS_LIB_DECL std::size_t globalNode2() const;
  MORFEUS_LIB_DECL std::size_t id() const;
  MORFEUS_LIB_DECL Type type() const;
  MORFEUS_LIB_DECL int unknown() const;
  MORFEUS_LIB_DECL void setGlobalNode1(std::size_t globalNode);
  MORFEUS_LIB_DECL void setGlobalNode2(std::size_t globalNode);
  MORFEUS_LIB_DECL void setType(Type type);
  MORFEUS_LIB_DECL void setUnknown(int unknown);

private:
  std::size_t mId;
  std::size_t mNode1;
  std::size_t mNode2;
  Type mType;
  int mUnknown;
};

#endif // EDGE_H
