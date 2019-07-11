#ifndef MESH_H
#define MESH_H

#include "morfeus.h"
#include "types.h"

#include <list>
#include <map>
#include <memory>
#include <vector>

class Edge;
class Element;
class Node;

class Mesh
{
public:
  MORFEUS_LIB_DECL Mesh();
  MORFEUS_LIB_DECL virtual ~Mesh();

public:
  MORFEUS_LIB_DECL int32_t totalEdges() const;
  MORFEUS_LIB_DECL int32_t totalElements() const;
  MORFEUS_LIB_DECL int32_t totalNodes() const;

public:
  MORFEUS_LIB_DECL void addEdge(std::unique_ptr<Edge> edge);
  MORFEUS_LIB_DECL void addElement(std::unique_ptr<Element> element);
  MORFEUS_LIB_DECL void addNode(std::unique_ptr<Node> node);
  MORFEUS_LIB_DECL Edge * edge(int index);
  MORFEUS_LIB_DECL const Edge * edge(int index) const;
  MORFEUS_LIB_DECL Element * element(int index);
  MORFEUS_LIB_DECL const Element * element(int index) const;
  MORFEUS_LIB_DECL Node * node(int index);
  MORFEUS_LIB_DECL const Node * node(int index) const;
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
  EdgeVector mEdges;
  ElementVector mElements;
  NodeVector mNodes;
  EdgeTable mEdgeTable;
};

#endif // MESH_H
