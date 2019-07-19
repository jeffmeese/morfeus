#ifndef FACE_H
#define FACE_H

#include "morfeus.h"

#include <vector>

class Mesh;

class Face
{
public:
  MORFEUS_LIB_DECL virtual ~Face();

public:
  MORFEUS_LIB_DECL int32_t id() const;
  MORFEUS_LIB_DECL int32_t edge(std::size_t index) const;
  MORFEUS_LIB_DECL int32_t node(std::size_t index) const;
  MORFEUS_LIB_DECL void setEdge(std::size_t index, int32_t value);
  MORFEUS_LIB_DECL void setNode(std::size_t index, int32_t value);
  MORFEUS_LIB_DECL std::size_t totalEdges() const;
  MORFEUS_LIB_DECL std::size_t totalNodes() const;

public:
  MORFEUS_LIB_DECL double computeArea(const Mesh * mesh) const;
  MORFEUS_LIB_DECL bool intersects(const Face * face) const;
  MORFEUS_LIB_DECL dcomplex computeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const;
  //MORFEUS_LIB_DECL void computeNormal(const Mesh & mesh) const;

protected:
  Face(int32_t id, std::size_t totalNodes, std::size_t totalEdges);

protected:
  virtual double doComputeArea(const Mesh * mesh) const = 0;
  virtual dcomplex doComputeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const = 0;
  virtual bool doIntersects(const Face * face) const = 0;

private:
  int32_t mId;
  std::vector<int32_t> mNodes;
  std::vector<int32_t> mEdges;
};

#endif // FACE_H
