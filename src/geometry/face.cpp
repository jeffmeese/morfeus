#include "face.h"

namespace morfeus {
namespace geometry {

Face::Face()
{
  mMarker = 0;
}

void Face::addSegment(const Segment &s)
{
  mSegments.push_back(s);
}

}
}
