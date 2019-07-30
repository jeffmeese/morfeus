#ifndef MORFEUS_GEOMETRY_FACE_H
#define MORFEUS_GEOMETRY_FACE_H

#include "morfeus.h"
#include "segment.h"

#include <vector>

namespace Morfeus {
namespace Geometry {

class Face
{
public:
  MORFEUS_LIB_DECL Face();

public:
  MORFEUS_LIB_DECL int marker() const;
  MORFEUS_LIB_DECL void setMarker(int value);

public:
  MORFEUS_LIB_DECL void addSegment(const Segment & s);
  MORFEUS_LIB_DECL const Segment & segment(std::size_t index) const;
  MORFEUS_LIB_DECL std::size_t totalSegments() const;

private:
  int mMarker;
  std::vector<Segment> mSegments;
};

inline int Face::marker() const
{
  return mMarker;
}

inline const Segment & Face::segment(std::size_t index) const
{
  return mSegments.at(index);
}

inline void Face::setMarker(int value)
{
  mMarker = value;
}

inline std::size_t Face::totalSegments() const
{
  return mSegments.size();
}

}
}

#endif // FACET_H
