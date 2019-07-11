#ifndef ELEMENT_H
#define ELEMENT_H

#include "morfeus.h"
#include "types.h"

#include <complex>
#include <vector>

class Face;
class Mesh;

class Element
{
public:
  MORFEUS_LIB_DECL virtual ~Element();

public:
  MORFEUS_LIB_DECL double attribute(int32_t index) const;
  MORFEUS_LIB_DECL int32_t id() const;
  MORFEUS_LIB_DECL int32_t edge(int32_t index) const;
  MORFEUS_LIB_DECL int32_t node(int32_t index) const;
  MORFEUS_LIB_DECL void setEdge(int32_t index, int32_t value);
  MORFEUS_LIB_DECL void setNode(int32_t index, int32_t value);
  MORFEUS_LIB_DECL int32_t totalAttributes() const;
  MORFEUS_LIB_DECL int32_t totalEdges() const;
  MORFEUS_LIB_DECL int32_t totalFaces() const;
  MORFEUS_LIB_DECL int32_t totalNodes() const;

public:
  MORFEUS_LIB_DECL void addAttribute(double value);
  MORFEUS_LIB_DECL void edgeNodes(int32_t index, int32_t & localNode1, int32_t & localNode2) const;
  MORFEUS_LIB_DECL Face * constructFace(int32_t index) const;
  MORFEUS_LIB_DECL dcomplex computeFeEntry(const Mesh & mesh, int32_t localEdge1, int32_t localEdge2) const;

protected:
  virtual dcomplex doComputeFeEntry(const Mesh & mesh, int32_t localEdge1, int32_t localEdge2) const = 0;
  virtual void doEdgeNodes(int32_t index, int32_t & node1, int32_t & node2) const = 0;
  virtual Face * doConstructFace(int32_t index) const = 0;

protected:
  Element(int32_t id, int32_t nodes, int32_t edges, int32_t faces);

private:
  int32_t mId;
  int32_t mFaces;
  std::vector<int32_t> mEdges;
  std::vector<int32_t> mNodes;
  std::vector<double> mAttributes;
};

#endif // ELEMENT_H
