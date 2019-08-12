#ifndef MORFEUS_EXCITATIONS_WIREPORT_H
#define MORFEUS_EXCITATIONS_WIREPORT_H

#include "port.h"

namespace morfeus {
  namespace model {
    namespace geometry {
      class Segment;
    }
  }
}

namespace morfeus {
namespace model {
namespace ports {

class WirePort
    : public Port
{
public:
  MORFEUS_LIB_DECL WirePort();
  MORFEUS_LIB_DECL WirePort(const std::string & name);
  MORFEUS_LIB_DECL WirePort(const std::string & id, const std::string & name);

public:
  MORFEUS_LIB_DECL const geometry::Segment * segment() const;
  MORFEUS_LIB_DECL void setSegment(const geometry::Segment * segment);

private:
  const geometry::Segment * mSegment;
};

inline const geometry::Segment * WirePort::segment() const
{
  return mSegment;
}

inline void WirePort::setSegment(const geometry::Segment * segment)
{
  mSegment = segment;
}

}
}
}

#endif // MORFEUS_EXCITATIONS_WIREPORT_H
