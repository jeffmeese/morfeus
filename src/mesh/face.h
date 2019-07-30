#ifndef FACE_H
#define FACE_H

#include "morfeus.h"
#include "morfeusobject.h"

#include <vector>

#include <boost/numeric/ublas/vector.hpp>

namespace Morfeus {
  class Planewave;
}

namespace Morfeus {
namespace mesh {

class Mesh;

class Face
    : public MorfeusObject
{
public:
  struct FarFieldEntry
  {
    dcomplex i1;
    dcomplex i2;
  };

public:
  MORFEUS_LIB_DECL int32_t edge(std::size_t index) const;
  MORFEUS_LIB_DECL int32_t edgeSign(std::size_t localEdge) const;
  MORFEUS_LIB_DECL int32_t node(std::size_t index) const;
  MORFEUS_LIB_DECL int32_t number() const;
  MORFEUS_LIB_DECL void setEdge(std::size_t index, int32_t value);
  MORFEUS_LIB_DECL void setEdgeSign(std::size_t localEdge, int32_t edgeSign);
  MORFEUS_LIB_DECL void setNode(std::size_t index, int32_t value);
  MORFEUS_LIB_DECL void setNumber(int32_t value);
  MORFEUS_LIB_DECL std::size_t totalEdges() const;
  MORFEUS_LIB_DECL std::size_t totalNodes() const;

protected:
  typedef boost::numeric::ublas::vector<dcomplex> vector;

public:
  MORFEUS_LIB_DECL double computeArea(const Mesh * mesh) const;
  MORFEUS_LIB_DECL double computeNormal(const Mesh * mesh) const;
  MORFEUS_LIB_DECL dcomplex computeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const;
  MORFEUS_LIB_DECL FarFieldEntry computeFarFieldEntry(double freqGhz, double theta, double phi, const Mesh* mesh, const vector & efield) const;
  MORFEUS_LIB_DECL dcomplex computePlanewaveEntry(std::size_t edge, double freq, double alpha, double thetaInc, double phiInc, const Mesh * mesh) const;
  MORFEUS_LIB_DECL bool isCoincident(const Face * otherFace) const;
  //MORFEUS_LIB_DECL void computeNormal(const Mesh & mesh) const;

protected:
  Face(const std::string & type, std::size_t totalNodes, std::size_t totalEdges);
  Face(const std::string & type, int32_t number, std::size_t totalNodes, std::size_t totalEdges);

protected:
  virtual double doComputeArea(const Mesh * mesh) const = 0;
  virtual dcomplex doComputeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const = 0;
  virtual dcomplex doComputePlanewaveEntry(std::size_t edge, double freq, double alpha, double theta, double phi, const Mesh * mesh) const = 0;

private:
  void init(std::size_t totalNodes, std::size_t totalEdges);

private:
  int32_t mNumber;
  std::vector<int32_t> mNodes;
  std::vector<int32_t> mEdges;
  std::vector<int32_t> mEdgeSigns;
};

inline int32_t Face::edgeSign(std::size_t localEdge) const
{
  return mEdgeSigns.at(localEdge);
}

inline int32_t Face::number() const
{
  return mNumber;
}

inline int32_t Face::edge(std::size_t index) const
{
  return mEdges[index];
}

inline int32_t Face::node(std::size_t index) const
{
  return mNodes[index];
}

inline void Face::setEdge(std::size_t index, int32_t value)
{
  mEdges[index] = value;
}

inline void Face::setEdgeSign(std::size_t localEdge, int32_t edgeSign)
{
  mEdgeSigns[localEdge] = edgeSign;
}

inline void Face::setNode(std::size_t index, int32_t value)
{
  mNodes[index] = value;
}

inline void Face::setNumber(int32_t value)
{
  mNumber = value;
}

inline std::size_t Face::totalEdges() const
{
  return mEdges.size();
}

inline std::size_t Face::totalNodes() const
{
  return mNodes.size();
}

}
}

#endif // FACE_H
