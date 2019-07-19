#ifndef MESH_H
#define MESH_H

#include "morfeus.h"

#include "mesh/node.h"
#include "mesh/edge.h"
#include "mesh/element.h"

#include "material/material.h"

#include <vector>
#include <list>
#include <map>
#include <memory>

class Mesh
{
public:
  MORFEUS_LIB_DECL Mesh();

public:
  MORFEUS_LIB_DECL double maxX() const;
  MORFEUS_LIB_DECL double maxY() const;
  MORFEUS_LIB_DECL double maxZ() const;
  MORFEUS_LIB_DECL double minX() const;
  MORFEUS_LIB_DECL double minY() const;
  MORFEUS_LIB_DECL double minZ() const;
  MORFEUS_LIB_DECL std::size_t totalEdges() const;
  MORFEUS_LIB_DECL std::size_t totalElements() const;
  MORFEUS_LIB_DECL std::size_t totalMaterials() const;
  MORFEUS_LIB_DECL std::size_t totalNodes() const;

public:
  MORFEUS_LIB_DECL void addEdge(std::unique_ptr<Edge> edge);
  MORFEUS_LIB_DECL void addElement(std::unique_ptr<Element> element);
  MORFEUS_LIB_DECL void addMaterial(std::unique_ptr<Material> material);
  MORFEUS_LIB_DECL void addNode(std::unique_ptr<Node> node);
  MORFEUS_LIB_DECL Edge * findEdge(std::size_t node1, std::size_t node2) const;
  MORFEUS_LIB_DECL Material * findMaterial(std::size_t id) const;
  MORFEUS_LIB_DECL std::size_t findNearestNode(double x, double y, double z) const;
  MORFEUS_LIB_DECL std::vector<std::unique_ptr<Face>> getBoundaryFaces() const;
  MORFEUS_LIB_DECL Edge * getEdge(std::size_t index) const;
  MORFEUS_LIB_DECL Element * getElement(std::size_t index) const;
  MORFEUS_LIB_DECL Material * getMaterial(std::size_t index) const;
  MORFEUS_LIB_DECL Node * getNode(std::size_t index) const;
  MORFEUS_LIB_DECL std::vector<Element*> getSurfaceElements() const;
  MORFEUS_LIB_DECL std::vector<Node*> getSurfaceNodes() const;
  //MORFEUS_LIB_DECL std::vector<std::unique_ptr<Face>> importGrdFile(const std::string & fileName);

private:
  typedef std::unique_ptr<Node> NodePtr;
  typedef std::unique_ptr<Edge> EdgePtr;
  typedef std::unique_ptr<Element> ElementPtr;
  typedef std::unique_ptr<Material> MaterialPtr;
  typedef std::vector<NodePtr> NodeVector;
  typedef std::vector<EdgePtr> EdgeVector;
  typedef std::vector<ElementPtr> ElementVector;
  typedef std::vector<MaterialPtr> MaterialVector;
  typedef NodeVector::iterator NodeItr;
  typedef EdgeVector::iterator EdgeItr;
  typedef ElementVector::iterator ElementItr;
  typedef NodeVector::const_iterator const_NodeItr;
  typedef EdgeVector::const_iterator const_EdgeItr;
  typedef ElementVector::const_iterator const_ElementItr;
  typedef std::pair<size_t, size_t> EdgePair;
  typedef std::list<EdgePair> EdgeList;
  typedef std::map<std::size_t, EdgeList> EdgeTable;

private:
  NodeVector mNodes;
  EdgeVector mEdges;
  ElementVector mElements;
  MaterialVector mMaterials;
  EdgeTable mEdgeTable;
  double mMinX;
  double mMaxX;
  double mMinY;
  double mMaxY;
  double mMinZ;
  double mMaxZ;
  double mNodeTolerance;
};

#endif // MESH_H
