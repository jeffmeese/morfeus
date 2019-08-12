#include "convexhull.h"

#include "face.h"
#include "node.h"

#include "model/media/medium.h"

#include <fstream>

namespace morfeus {
namespace mesh {

ConvexHull::ConvexHull()
{
}

void ConvexHull::addFace(const Face *face)
{
  mFaces.push_back(face);
}

void ConvexHull::addNode(const Node *node)
{
  mNodes.push_back(node);
}

void ConvexHull::writeVtkFile(const std::string &fileName) const
{
  std::ofstream output(fileName);

  output << "# vtk DataFile Version 2.0\n";
  output << "Unstructured Grid\n";
  output << "ASCII\n";
  output << "DATASET UNSTRUCTURED_GRID\n";
  output << "POINTS " << mNodes.size() << " double\n";
  std::size_t nodeIndex = 0;
  for (std::size_t i = 0; i < mNodes.size(); i++) {
    const Node * node = mNodes.at(i);
    output << node->x() << " " << node->y() << " " << node->z() << "\n";
    nodeIndex += 3;
  }
  output << "\n";

  output << "CELLS " << mFaces.size() << " " << mFaces.size()*4 << "\n";
  for (std::size_t i = 0; i < mFaces.size(); i++) {
    const Face * face = mFaces.at(i);
    output << face->totalNodes() << " ";
    for (std::size_t j = 0; j < face->totalNodes(); j++) {
      int32_t number = face->node(j);
      output << number << " ";
    }
    output << "\n";
  }
  output << "\n";

  output << "CELL_TYPES " << mFaces.size() << "\n";
  for (std::size_t i = 0; i < mFaces.size(); i++) {
    output << "5\n";
  }
  output << "\n";

  output << "CELL_DATA " << mFaces.size() << "\n";
  output << "SCALARS cell_scalars int 1\n";
  output << "LOOKUP_TABLE default\n";
  for (std::size_t i = 0; i < mFaces.size(); i++) {
    const Face * face = mFaces.at(i);
    int32_t attribute = 0;
    if (face->medium() != nullptr) {
      attribute = face->medium()->attribute();
    }
    output << attribute << "\n";
  }
  output << "\n";
}

}
}
