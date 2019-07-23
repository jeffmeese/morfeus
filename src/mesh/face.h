#ifndef FACE_H
#define FACE_H

#include "morfeus.h"

#include <vector>

#include <boost/numeric/ublas/vector.hpp>

class Planewave;
class Mesh;

class Face
{
public:
  enum class Position
  {
    Top = 0,
    Side = 1,
    Bottom = 2
  };

  struct FarFieldEntry
  {
    dcomplex i1;
    dcomplex i2;
  };

public:
  MORFEUS_LIB_DECL virtual ~Face();

public:
  MORFEUS_LIB_DECL int32_t id() const;
  MORFEUS_LIB_DECL int32_t edge(std::size_t index) const;
  MORFEUS_LIB_DECL int32_t edgeSign(std::size_t localEdge) const;
  MORFEUS_LIB_DECL int32_t node(std::size_t index) const;
  MORFEUS_LIB_DECL Position position() const;
  MORFEUS_LIB_DECL void setEdge(std::size_t index, int32_t value);
  MORFEUS_LIB_DECL void setEdgeSign(std::size_t localEdge, int32_t edgeSign);
  MORFEUS_LIB_DECL void setNode(std::size_t index, int32_t value);
  MORFEUS_LIB_DECL void setPosition(Position position);
  MORFEUS_LIB_DECL std::size_t totalEdges() const;
  MORFEUS_LIB_DECL std::size_t totalNodes() const;

protected:
  typedef boost::numeric::ublas::vector<dcomplex> vector;

public:
  MORFEUS_LIB_DECL double computeArea(const Mesh * mesh) const;
  MORFEUS_LIB_DECL double computeNormal(const Mesh * mesh) const;
  MORFEUS_LIB_DECL dcomplex computeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const;
  MORFEUS_LIB_DECL FarFieldEntry computeFarFieldEntry(double freqGhz, double theta, double phi, const Mesh* mesh, const vector & efield) const;
  MORFEUS_LIB_DECL dcomplex computePlanewaveEntry(std::size_t edge, double freq, const Planewave * planewave, const Mesh * mesh) const;
  MORFEUS_LIB_DECL bool isCoincident(const Face * otherFace) const;
  //MORFEUS_LIB_DECL void computeNormal(const Mesh & mesh) const;

protected:
  Face(int32_t id, std::size_t totalNodes, std::size_t totalEdges);

protected:
  virtual double doComputeArea(const Mesh * mesh) const = 0;
  virtual dcomplex doComputeMomEntry(const Face * otherFace, const Mesh * mesh, std::size_t localEdge) const = 0;
  virtual dcomplex doComputePlanewaveEntry(std::size_t edge, double freq, const Planewave * planewave, const Mesh * mesh) const = 0;

private:
  int32_t mId;
  Position mPosition;
  std::vector<int32_t> mNodes;
  std::vector<int32_t> mEdges;
  std::vector<int32_t> mEdgeSigns;
};

#endif // FACE_H
