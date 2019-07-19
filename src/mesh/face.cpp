#include "face.h"

#include "mesh.h"

Face::Face(int32_t id, std::size_t totalNodes, std::size_t totalEdges)
  : mId(id)
{
  mNodes.resize(totalNodes);
  mEdges.resize(totalEdges);

  for (std::size_t i = 0; i < totalNodes; i++) {
    mNodes[i] = -1;
  }
  for (std::size_t i = 0; i < totalEdges; i++) {
    mEdges[i] = -1;
  }
}

Face::~Face()
{
}

double Face::computeArea(const Mesh * mesh) const
{
  return doComputeArea(mesh);
}

dcomplex Face::computeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const
{
  return doComputeMomEntry(otherFace, mesh, localEdge);
}

int32_t Face::id() const
{
  return mId;
}

bool Face::intersects(const Face *face) const
{
  // We assume faces with different numbers of nodes or edges can't intersect
  if (face->totalEdges() != this->totalEdges() || face->totalNodes() != this->totalNodes()) {
    return false;
  }

  return doIntersects(face);
}

int32_t Face::edge(std::size_t index) const
{
  return mEdges[index];
}

int32_t Face::node(std::size_t index) const
{
  return mNodes[index];
}

void Face::setEdge(std::size_t index, int32_t value)
{
  mEdges[index] = value;
}

void Face::setNode(std::size_t index, int32_t value)
{
  mNodes[index] = value;
}

std::size_t Face::totalEdges() const
{
  return mEdges.size();
}

std::size_t Face::totalNodes() const
{
  return mNodes.size();
}
