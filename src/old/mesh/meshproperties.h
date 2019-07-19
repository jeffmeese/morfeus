#ifndef MESHPROPERTIES_H
#define MESHPROPERTIES_H

#include "morfeus.h"

#include <cstdlib>
#include <vector>

class Edge;
class Node;
class Element;
class Mesh;

class MeshProperties
{
public:
  MORFEUS_LIB_DECL MeshProperties(const Mesh & mesh);

public:
  MORFEUS_LIB_DECL std::size_t totalApertureEdges() const;
  //MORFEUS_LIB_DECL std::size_t totalApertureElements() const;
  MORFEUS_LIB_DECL std::size_t totalApertureNodes() const;
  MORFEUS_LIB_DECL std::size_t totalApertureUnknowns() const;
  MORFEUS_LIB_DECL std::size_t totalUnknowns() const;
  MORFEUS_LIB_DECL const std::vector<Edge*> & apertureEdges() const;
  MORFEUS_LIB_DECL const std::vector<Node*> & apertureNodes() const;

public:
  MORFEUS_LIB_DECL void rescan(const Mesh & mesh);

private:
  std::size_t mTotalApertureEdges;
  //std::size_t mTotalApertureElements;
  std::size_t mTotalApertureNodes;
  std::size_t mTotalApertureUnknowns;
  std::size_t mTotalUnknowns;
  std::vector<Edge*> mApertureEdges;
  std::vector<Node*> mApertureNodes;
};

#endif // MESHPROPERTIES_H
