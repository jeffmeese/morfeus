#include "face.h"

#include "mesh.h"

Face::Face(int32_t id, int32_t totalNodes, int32_t totalEdges)
  : mId(id)
  , mNumEdges(totalEdges)
  , mNumNodes(totalNodes)
{
  mNodes = new int32_t[mNumEdges];
  mEdges = new int32_t[mNumNodes];
}

Face::~Face()
{
  delete [] mNodes;
  delete [] mEdges;
}

double Face::computeArea(const Mesh & mesh) const
{
  return doComputeArea(mesh);
}

int32_t Face::id() const
{
  return mId;
}

bool Face::intersects(const Face *face) const
{
  return false;
}

int32_t Face::edge(int32_t index) const
{
  return mEdges[index-1];
}

int32_t Face::node(int32_t index) const
{
  return mNodes[index-1];
}

void Face::setEdge(int32_t index, int32_t value)
{
  mEdges[index-1] = value;
}

void Face::setNode(int32_t index, int32_t value)
{
  mNodes[index-1] = value;
}

int32_t Face::totalEdges() const
{
  return mNumEdges;
}

int32_t Face::totalNodes() const
{
  return mNumNodes;
}
