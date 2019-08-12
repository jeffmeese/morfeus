#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include "morfeus.h"

#include <vector>

namespace morfeus {
  namespace mesh {
    class Face;
    class Node;
  }
}

namespace morfeus {
namespace mesh {

class ConvexHull
{
public:
  MORFEUS_LIB_DECL ConvexHull();

public:
  MORFEUS_LIB_DECL void addFace(const Face * face);
  MORFEUS_LIB_DECL void addNode(const Node * node);
  MORFEUS_LIB_DECL void writeVtkFile(const std::string & fileName) const;

private:
  std::vector<const Face*> mFaces;
  std::vector<const Node*> mNodes;
};

}
}

#endif // CONVEXHULL_H
