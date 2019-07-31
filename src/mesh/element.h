#ifndef MORFEUS_MESH_ELEMENT_H
#define MORFEUS_MESH_ELEMENT_H

#include "morfeus.h"

#include "core/morfeusobject.h"

#include <vector>

namespace morfeus {
  namespace mesh {
    class Face;
    class Mesh;
  }
}

namespace morfeus {
namespace mesh {

class Element
    : core::MorfeusObject
{
public:
  // Element attributes
  static const int MetallicBoundary = 0;
  static const int MetallicElement = 1;
  static const int DielectricElement = 2;

public:
  MORFEUS_LIB_DECL int32_t attribute() const;
  MORFEUS_LIB_DECL int32_t edgeSign(std::size_t index) const;
  MORFEUS_LIB_DECL int32_t edge(std::size_t index) const;
  MORFEUS_LIB_DECL int32_t epsilonId() const;
  MORFEUS_LIB_DECL int32_t muId() const;
  MORFEUS_LIB_DECL int32_t node(std::size_t index) const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL void setAttribute(int32_t value);
  MORFEUS_LIB_DECL void setEpsilonId(int32_t value);
  MORFEUS_LIB_DECL void setEdge(std::size_t index, int32_t value);
  MORFEUS_LIB_DECL void setEdgeSign(std::size_t index, int32_t value);
  MORFEUS_LIB_DECL void setMuId(int32_t value);
  MORFEUS_LIB_DECL void setNode(std::size_t index, int32_t value);
  MORFEUS_LIB_DECL void setNumber(int32_t number);
  MORFEUS_LIB_DECL std::size_t totalEdges() const;
  MORFEUS_LIB_DECL std::size_t totalFaces() const;
  MORFEUS_LIB_DECL std::size_t totalNodes() const;

public:
  MORFEUS_LIB_DECL void edgeNodes(std::size_t index, int32_t & localNode1, int32_t & localNode2) const;
  MORFEUS_LIB_DECL void computeFeEntry(const Mesh * mesh, std::size_t localEdge1, std::size_t localEdge2, dcomplex & i1, dcomplex & i2) const;
  MORFEUS_LIB_DECL Face * getFace(std::size_t index) const;

protected:
  virtual void doComputeFeEntry(const Mesh * mesh, std::size_t localEdge1, std::size_t localEdge2, dcomplex & i1, dcomplex & i2) const = 0;
  virtual void doEdgeNodes(std::size_t index, int32_t & node1, int32_t & node2) const = 0;
  virtual Face * doGetFace(std::size_t index) const = 0;

protected:
  Element(const std::string & type, std::size_t nodes, std::size_t edges, std::size_t faces);
  Element(const std::string & type, int32_t number, std::size_t nodes, std::size_t edges, std::size_t faces);

private:
  void init(std::size_t nodes, std::size_t edges, std::size_t faces);

private:
  int32_t mAttribute;
  int32_t mNumber;
  int32_t mEpsilonId;
  int32_t mMuId;
  std::size_t mFaces;
  std::vector<int32_t> mEdges;
  std::vector<int32_t> mEdgeSigns;
  std::vector<int32_t> mNodes;
};

inline int32_t Element::attribute() const
{
  return mAttribute;
}

inline int32_t Element::edge(std::size_t index) const
{
  return mEdges.at(index);
}

inline int32_t Element::edgeSign(std::size_t index) const
{
  return mEdgeSigns.at(index);
}

inline int32_t Element::epsilonId() const
{
  return mEpsilonId;
}

inline int32_t Element::muId() const
{
  return mMuId;
}

inline int32_t Element::node(std::size_t index) const
{
  return mNodes.at(index);
}

inline int32_t Element::number() const
{
  return mNumber;
}

inline void Element::setAttribute(int32_t value)
{
  mAttribute = value;
}

inline void Element::setEdge(std::size_t index, int32_t value)
{
  mEdges[index] = value;
}

inline void Element::setEdgeSign(std::size_t index, int32_t value)
{
  mEdgeSigns[index] = value;
}

inline void Element::setEpsilonId(int32_t value)
{
  mEpsilonId = value;
}

inline void Element::setMuId(int32_t value)
{
  mMuId = value;
}

inline void Element::setNode(std::size_t index, int32_t value)
{
  mNodes[index] = value;
}

inline void Element::setNumber(int32_t number)
{
  mNumber = number;
}

inline std::size_t Element::totalEdges() const
{
  return mEdges.size();
}

inline std::size_t Element::totalFaces() const
{
  return mFaces;
}

inline std::size_t Element::totalNodes() const
{
  return mNodes.size();
}

}
}

#endif // MORFEUS_MESH_ELEMENT_H
