#ifndef ELEMENT_H
#define ELEMENT_H

#include "morfeus.h"
#include "types.h"

#include <cstddef>
#include <vector>
#include <memory>

class Face;
class Mesh;
class Material;

class Element
{
public:
  enum class Type
  {
    Antenna = 1,
    Metal = 2,
    Dielectric = 5
  };

public:
  MORFEUS_LIB_DECL int edgeSign(std::size_t localEdge) const;
  MORFEUS_LIB_DECL std::size_t globalEdge(std::size_t localEdge) const;
  MORFEUS_LIB_DECL std::size_t globalNode(std::size_t localNode) const;
  MORFEUS_LIB_DECL std::size_t id() const;
  MORFEUS_LIB_DECL std::size_t epsId() const;
  MORFEUS_LIB_DECL std::size_t muId() const;
  MORFEUS_LIB_DECL std::size_t section() const;
  MORFEUS_LIB_DECL std::size_t totalFaces() const;
  MORFEUS_LIB_DECL std::size_t totalEdges() const;
  MORFEUS_LIB_DECL std::size_t totalNodes() const;
  MORFEUS_LIB_DECL Type type() const;
  MORFEUS_LIB_DECL void setEdgeSign(std::size_t localEdge, int edgeSign);
  MORFEUS_LIB_DECL void setEpsId(std::size_t epsId);
  MORFEUS_LIB_DECL void setMuId(std::size_t muId);
  MORFEUS_LIB_DECL void setGlobalEdge(std::size_t localEdge, std::size_t globalEdge);
  MORFEUS_LIB_DECL void setGlobalNode(std::size_t localNode, std::size_t globalNode);
  MORFEUS_LIB_DECL void setSection(std::size_t section);
  MORFEUS_LIB_DECL void setType(Type type);

public:
  virtual dcomplex computeMatrixEntry(const Material * eps, const Material * mu, const Mesh & mesh, double freq, std::size_t source, std::size_t test) const = 0;
  virtual std::unique_ptr<Element> copy(size_t newId) const = 0;
  virtual std::unique_ptr<Face> createFace(std::size_t localFace) const = 0;
  virtual void getEdgeNodes(std::size_t localEdge, std::size_t & node1, std::size_t & node2) const = 0;

protected:
  Element(std::size_t id, std::size_t totalNodes, std::size_t totalEdges, std::size_t totalFaces);

private:
  std::size_t mId;
  std::size_t mEpsId;
  std::size_t mMuId;
  std::size_t mSection;
  std::size_t mTotalFaces;
  Type mType;
  std::vector<int> mEdgeSigns;
  std::vector<std::size_t> mGlobalEdges;
  std::vector<std::size_t> mGlobalNodes;
};

#endif // ELEMENT_H
