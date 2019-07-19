#ifndef FACE_H
#define FACE_H

#include "morfeus.h"
#include "types.h"

#include "math/vector.h"

#include <cstddef>
#include <vector>
#include <memory>

class Element;
class Mesh;
class Planewave;

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
  MORFEUS_LIB_DECL Face(std::size_t nodes, std::size_t edges);

public:
  MORFEUS_LIB_DECL int edgeSign(std::size_t localEdge) const;
  MORFEUS_LIB_DECL std::size_t globalEdge(std::size_t localEdge) const;
  MORFEUS_LIB_DECL std::size_t globalNode(std::size_t localNode) const;
  MORFEUS_LIB_DECL Position position() const;
  MORFEUS_LIB_DECL std::size_t sectionId() const;
  MORFEUS_LIB_DECL std::size_t totalEdges() const;
  MORFEUS_LIB_DECL std::size_t totalNodes() const;
  MORFEUS_LIB_DECL void setEdgeSign(std::size_t localEdge, int edgeSign);
  MORFEUS_LIB_DECL void setGlobalEdge(std::size_t localEdge, std::size_t globalEdge);
  MORFEUS_LIB_DECL void setGlobalNode(std::size_t localNode, std::size_t globalNode);
  MORFEUS_LIB_DECL void setPosition(Position position);
  MORFEUS_LIB_DECL void setSectionId(std::size_t id);

public:
  MORFEUS_LIB_DECL math::vector<double> computeNormal(const Mesh & mesh) const;
  MORFEUS_LIB_DECL bool isCoincident(const Face * otherFace) const;

public:
  virtual FarFieldEntry computeFarFieldEntry(double freq, double theta, double phi, const dcomplex_array & efield, const Mesh & mesh) const = 0;
  virtual dcomplex computePlanewaveEntry(std::size_t localEdge, double freq, const Planewave * planewave, const Mesh & mesh) const = 0;
  virtual std::unique_ptr<Element> extrudeElement(std::size_t id, std::size_t nodeOffset) const = 0;
  virtual double computeArea(const Mesh & mesh) const = 0;

private:
  std::vector<std::size_t> mGlobalEdges;
  std::vector<std::size_t> mGlobalNodes;
  std::vector<int> mEdgeSigns;
  Position mPosition;
  std::size_t mSectionId;
};

#endif // FACE_H
