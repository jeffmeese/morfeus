#ifndef EDGE_H
#define EDGE_H

#include "morfeus.h"
#include "morfeusobject.h"

namespace morfeus {
namespace mesh {

  class Mesh;

class Edge
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL Edge(int32_t number);
  MORFEUS_LIB_DECL Edge(int32_t number, int32_t node1, int32_t node2);

public:
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL int32_t node1() const;
  MORFEUS_LIB_DECL int32_t node2() const;
  MORFEUS_LIB_DECL int32_t unknownNumber() const;
  MORFEUS_LIB_DECL void setNode1(int32_t value);
  MORFEUS_LIB_DECL void setNode2(int32_t value);
  MORFEUS_LIB_DECL void setUnknownNumber(int32_t value);

public:
  MORFEUS_LIB_DECL double computeLength(const Mesh * mesh) const;

private:
  int32_t mNumber;
  int32_t mNode1;
  int32_t mNode2;
  int32_t mUnknown;
};

inline int32_t Edge::number() const
{
  return mNumber;
}

inline int32_t Edge::node1() const
{
  return mNode1;
}

inline int32_t Edge::node2() const
{
  return mNode2;
}

inline void Edge::setNode1(int32_t value)
{
  mNode1 = value;
}

inline void Edge::setNode2(int32_t value)
{
  mNode2 = value;
}

inline void Edge::setUnknownNumber(int32_t value)
{
  mUnknown = value;
}

inline int32_t Edge::unknownNumber() const
{
  return mUnknown;
}

}
}

#endif // EDGE_H
