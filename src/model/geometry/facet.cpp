#include "facet.h"

namespace morfeus {
namespace model {
namespace geometry {

Facet::Facet()
{

}

void Facet::addVertex(const Vertex *vertex)
{
  mVertices.push_back(vertex);
}

}
}
}
