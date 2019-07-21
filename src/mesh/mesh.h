#ifndef MESH_H
#define MESH_H

#include "morfeus.h"

#include <list>
#include <map>
#include <memory>
#include <vector>

class Edge;
class Element;
class MaterialDatabase;
class Node;

class Mesh
{
public:
  MORFEUS_LIB_DECL Mesh();
  MORFEUS_LIB_DECL virtual ~Mesh();

public:
  MORFEUS_LIB_DECL const MaterialDatabase * materialDatabase() const;
  MORFEUS_LIB_DECL void setMaterialDatabase(const MaterialDatabase * database);
  MORFEUS_LIB_DECL std::size_t totalEdges() const;
  MORFEUS_LIB_DECL std::size_t totalElements() const;
  MORFEUS_LIB_DECL std::size_t totalNodes() const;

public:
  MORFEUS_LIB_DECL void addEdge(std::unique_ptr<Edge> edge);
  MORFEUS_LIB_DECL void addElement(std::unique_ptr<Element> element);
  MORFEUS_LIB_DECL void addNode(std::unique_ptr<Node> node);
  MORFEUS_LIB_DECL Edge * edge(std::size_t index);
  MORFEUS_LIB_DECL const Edge * edge(std::size_t index) const;
  MORFEUS_LIB_DECL Element * element(std::size_t index);
  MORFEUS_LIB_DECL const Element * element(std::size_t index) const;
  MORFEUS_LIB_DECL Edge * findEdge(std::size_t node1, std::size_t node2) const;
  MORFEUS_LIB_DECL int32_t findNearestNode(double x, double y, double z) const;
  MORFEUS_LIB_DECL Node * node(std::size_t index);
  MORFEUS_LIB_DECL const Node * node(std::size_t index) const;
  MORFEUS_LIB_DECL std::unique_ptr<Edge> takeEdge(std::size_t index);
  MORFEUS_LIB_DECL std::unique_ptr<Element> takeElement(std::size_t index);
  MORFEUS_LIB_DECL std::unique_ptr<Node> takeNode(std::size_t index);
  MORFEUS_LIB_DECL void writeGeomFile(const std::string & fileName) const;

  // Implementation
private:
  void createEdges(Element * element);

  // Type definitions
private:
  typedef std::unique_ptr<Edge> EdgePtr;
  typedef std::unique_ptr<Element> ElementPtr;
  typedef std::unique_ptr<Node> NodePtr;
  typedef std::vector<EdgePtr> EdgeVector;
  typedef std::vector<ElementPtr> ElementVector;
  typedef std::vector<NodePtr> NodeVector;
  typedef std::pair<int32_t, int32_t> EdgePair;
  typedef std::list<EdgePair> EdgeList;
  typedef std::map<int32_t, EdgeList> EdgeTable;

private:
  double mNodeTolerance;
  EdgeVector mEdges;
  ElementVector mElements;
  NodeVector mNodes;
  EdgeTable mEdgeTable;
  const MaterialDatabase * mMaterialDatabase;
};

inline const MaterialDatabase * Mesh::materialDatabase() const
{
  return mMaterialDatabase;
}

inline void Mesh::setMaterialDatabase(const MaterialDatabase *database)
{
  mMaterialDatabase = database;
}

#endif // MESH_H
