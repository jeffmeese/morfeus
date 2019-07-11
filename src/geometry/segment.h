#ifndef SEGMENT_H
#define SEGMENT_H

#include "morfeus.h"

class Segment
{
public:
  MORFEUS_LIB_DECL Segment(int32_t id);
  MORFEUS_LIB_DECL Segment(int32_t id, int32_t node1, int32_t node2);

public:
  MORFEUS_LIB_DECL bool boundary() const;
  MORFEUS_LIB_DECL int32_t id() const;
  MORFEUS_LIB_DECL int32_t node1() const;
  MORFEUS_LIB_DECL int32_t node2() const;
  MORFEUS_LIB_DECL void setBoundary(bool value);
  MORFEUS_LIB_DECL void setNode1(int32_t node1);
  MORFEUS_LIB_DECL void setNode2(int32_t node2);
  MORFEUS_LIB_DECL void setNodes(int32_t node1, int32_t node2);

private:
  bool mBoundary;
  int32_t mId;
  int32_t mNode1;
  int32_t mNode2;
};

#endif // SEGMENT_H
