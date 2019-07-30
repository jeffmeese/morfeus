#include "face.h"

namespace Morfeus {
namespace Geometry {

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
