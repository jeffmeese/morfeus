#ifndef FACE_H
#define FACE_H

#include "morfeus.h"
#include "types.h"

#include <vector>

class Mesh;

class Face
{
public:
  MORFEUS_LIB_DECL virtual ~Face();

public:
  MORFEUS_LIB_DECL int32_t id() const;
  MORFEUS_LIB_DECL int32_t edge(int32_t index) const;
  MORFEUS_LIB_DECL int32_t node(int32_t index) const;
  MORFEUS_LIB_DECL void setEdge(int32_t index, int32_t value);
  MORFEUS_LIB_DECL void setNode(int32_t index, int32_t value);
  MORFEUS_LIB_DECL int32_t totalEdges() const;
  MORFEUS_LIB_DECL int32_t totalNodes() const;

public:
  MORFEUS_LIB_DECL double computeArea(const Mesh & mesh) const;
  MORFEUS_LIB_DECL bool intersects(const Face * face) const;
  MORFEUS_LIB_DECL dcomplex computeMomEntry(const Face * otherFace, const Mesh & mesh, int32_t localEdge) const;
  //MORFEUS_LIB_DECL void computeNormal(const Mesh & mesh) const;

protected:
  Face(int id, int totalNodes, int totalEdges);

protected:
  virtual double doComputeArea(const Mesh & mesh) const = 0;
  virtual dcomplex doComputeMomEntry(const Face * otherFace, const Mesh & mesh, int32_t localEdge) const = 0;

private:
  int32_t mId;
  int32_t mNumEdges;
  int32_t mNumNodes;
  int32_t * mNodes;
  int32_t * mEdges;
};

#endif // FACE_H
