#include "meshinformation.h"

#include "face.h"
#include "mesh.h"

MeshInformation::MeshInformation()
{
  init();
}

void MeshInformation::analyzeMesh(const Mesh * mesh)
{
  init();
  identifyBoundaryFaces(mesh);
  markMeshUnknowns(mesh);
}

void MeshInformation::identifyBoundaryFaces(const Mesh *mesh)
{

}

void MeshInformation::init()
{
  mTotalUnknowns = mTotalBoundaryUnknowns = 0;
  mBoundaryFaces.clear();
}

void MeshInformation::markMeshUnknowns(const Mesh *mesh)
{

}
