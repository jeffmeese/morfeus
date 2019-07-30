#ifndef MESHINFORMATION_H
#define MESHINFORMATION_H

#include "morfeus.h"

#include <map>
#include <vector>

namespace Morfeus {
  namespace mesh {
    class Element;
    class Face;
    class Mesh;
  }
}

namespace Morfeus {

class MeshInformation
{
public:
  MORFEUS_LIB_DECL MeshInformation();

public:
  MORFEUS_LIB_DECL const std::vector<mesh::Face*> & boundaryFaces() const;
  MORFEUS_LIB_DECL std::size_t totalBoundaryUnknowns() const;
  MORFEUS_LIB_DECL std::size_t totalUnknowns() const;

public:
  MORFEUS_LIB_DECL void analyzeMesh(mesh::Mesh * mesh);

private:
  void identifyBoundaryFaces(mesh::Mesh * mesh);
  void init();
  void markMeshUnknowns(mesh::Mesh * mesh);

private:
  std::size_t mTotalBoundaryUnknowns;
  std::size_t mTotalUnknowns;
  std::vector<mesh::Face*> mBoundaryFaces;
  std::map<mesh::Face*, mesh::Element*> mFaceToElement;
};

inline const std::vector<mesh::Face*> & MeshInformation::boundaryFaces() const
{
  return mBoundaryFaces;
}

inline std::size_t MeshInformation::totalBoundaryUnknowns() const
{
  return mTotalBoundaryUnknowns;
}
inline std::size_t MeshInformation::totalUnknowns() const
{
  return mTotalUnknowns;
}

}

#endif // MESHINFORMATION_H
