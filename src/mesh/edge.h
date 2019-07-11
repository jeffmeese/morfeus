#ifndef EDGE_H
#define EDGE_H

#include "morfeus.h"
#include "types.h"

#include <vector>

class Edge
{
public:
  MORFEUS_LIB_DECL Edge(int32_t id);
  MORFEUS_LIB_DECL Edge(int32_t id, int32_t node1, int32_t node2);

public:
  MORFEUS_LIB_DECL int32_t id() const;
  MORFEUS_LIB_DECL int32_t node1() const;
  MORFEUS_LIB_DECL int32_t node2() const;
  MORFEUS_LIB_DECL void setNode1(int32_t value);
  MORFEUS_LIB_DECL void setNode2(int32_t value);

private:
  int32_t mId;
  int32_t mNode1;
  int32_t mNode2;
};

#endif // EDGE_H
