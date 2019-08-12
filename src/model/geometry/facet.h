#ifndef FACET_H
#define FACET_H

#include "morfeus.h"

#include <vector>

namespace morfeus {

  namespace model {
    namespace geometry {
      class Vertex;
    }
  }
}

namespace morfeus {
namespace model {
namespace geometry {

class Facet
{
public:
  MORFEUS_LIB_DECL Facet();

public:
  MORFEUS_LIB_DECL void addVertex(const Vertex * vertex);
  MORFEUS_LIB_DECL const Vertex * vertex(std::size_t index) const;
  MORFEUS_LIB_DECL std::size_t totalVertices() const;

private:
  std::vector<const Vertex *> mVertices;
};

inline const Vertex * Facet::vertex(std::size_t index) const
{
  return mVertices.at(index);
}

inline std::size_t Facet::totalVertices() const
{
  return mVertices.size();
}

}
}
}

#endif // FACET_H
