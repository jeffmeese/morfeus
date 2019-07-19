#ifndef SURFACEMESH_H
#define SURFACEMESH_H

#include <string>
#include <memory>
#include <vector>

#include "morfeus.h"
#include "node.h"
#include "edge.h"
#include "face.h"

class Mesh;
class Element;

class SurfaceMesh
{
public:
  MORFEUS_LIB_DECL SurfaceMesh();
  MORFEUS_LIB_DECL SurfaceMesh(const std::string & name);

public:
  MORFEUS_LIB_DECL std::string name() const;
  MORFEUS_LIB_DECL std::size_t totalEdges() const;
  MORFEUS_LIB_DECL std::size_t totalFaces() const;
  MORFEUS_LIB_DECL std::size_t totalNodes() const;
  MORFEUS_LIB_DECL void setName(const std::string & name);

public:
  MORFEUS_LIB_DECL void addEdge(std::unique_ptr<Edge> edge);
  MORFEUS_LIB_DECL void addFace(std::unique_ptr<Face> element);
  MORFEUS_LIB_DECL void addNode(std::unique_ptr<Node> node);
  MORFEUS_LIB_DECL void generateMesh(Mesh & mesh, double layerHeight) const;
  MORFEUS_LIB_DECL Edge * getEdge(std::size_t index) const;
  MORFEUS_LIB_DECL Face * getFace(std::size_t index) const;
  MORFEUS_LIB_DECL Node * getNode(std::size_t index) const;
  MORFEUS_LIB_DECL bool importGrdFile(const std::string & fileName);
  MORFEUS_LIB_DECL bool importGrdMeshFile(const std::string & fileName);

private:
  void generateElementEdges(Element * element, Mesh & mesh) const;

private:
  std::string mName;
  std::vector<std::unique_ptr<Node>> mNodes;
  std::vector<std::unique_ptr<Edge>> mEdges;
  std::vector<std::unique_ptr<Face>> mFaces;
};

#endif // SURFACEMESH_H
