#include "mesh.h"
#include "edge.h"
#include "element.h"
#include "node.h"

#include <fstream>
#include <sstream>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::addEdge(std::unique_ptr<Edge> edge)
{
  mEdgeTable[edge->node1()].push_back(EdgePair(edge->node2(), edge->id()));
  mEdgeTable[edge->node2()].push_back(EdgePair(edge->node1(), edge->id()));
  mEdges.push_back(std::move(edge));
}

void Mesh::addElement(std::unique_ptr<Element> element)
{
  createEdges(element.get());
  mElements.push_back(std::move(element));
}

void Mesh::addNode(std::unique_ptr<Node> node)
{
  mNodes.push_back(std::move(node));
}

void Mesh::createEdges(Element *element)
{
  for (int32_t i = 0; i < element->totalEdges(); i++) {
    int32_t edgeNode1 = -1, edgeNode2 = -1;
    element->edgeNodes(static_cast<int32_t>(i), edgeNode1, edgeNode2);
    int node1 = element->edge(edgeNode1);
    int node2 = element->edge(edgeNode2);

    int edgeNumber = -1;
    EdgeList & edgeList1 = mEdgeTable[node1];
    for (EdgeList::iterator itr = edgeList1.begin(); itr != edgeList1.end(); ++itr) {
      if (itr->first == node2) {
        edgeNumber = itr->second;
        break;
      }
    }

    EdgeList & edgeList2 = mEdgeTable[node2];
    for (EdgeList::iterator itr = edgeList2.begin(); itr != edgeList2.end(); ++itr) {
      if (itr->first == node1) {
        edgeNumber = itr->second;
        break;
      }
    }

    if (edgeNumber == -1) {
      edgeNumber = static_cast<int32_t>(mEdges.size() + 1);
      EdgePtr edge(new Edge(edgeNumber, node1, node2));
      addEdge(std::move(edge));
    }
  }
}

Edge * Mesh::edge(int index)
{
  return mEdges.at(static_cast<std::size_t>(index-1)).get();
}

const Edge * Mesh::edge(int index) const
{
  return mEdges.at(static_cast<std::size_t>(index-1)).get();
}

Element * Mesh::element(int index)
{
  return mElements.at(static_cast<std::size_t>(index-1)).get();
}

const Element * Mesh::element(int index) const
{
  return mElements.at(static_cast<std::size_t>(index-1)).get();
}

Node * Mesh::node(int index)
{
  return mNodes.at(static_cast<std::size_t>(index-1)).get();
}

const Node * Mesh::node(int index) const
{
  return mNodes.at(static_cast<std::size_t>(index-1)).get();
}

int32_t Mesh::totalEdges() const
{
  return static_cast<int32_t>(mEdges.size());
}

int32_t Mesh::totalElements() const
{
  return static_cast<int32_t>(mElements.size());
}

int32_t Mesh::totalNodes() const
{
  return static_cast<int32_t>(mNodes.size());
}

void Mesh::writeGeomFile(const std::string &fileName) const
{
  std::ofstream output(fileName);
  if (output.fail()) {
    std::ostringstream oss;
    oss << "Could not open geom file " << fileName;
    throw std::invalid_argument(oss.str());
  }

  output << this->totalNodes() << "\n";
  for (int i = 1; i <= this->totalNodes(); i++) {
    const Node * node = this->node(i);
    output << node->x() << " " << node->y() << " " << node->z() << "\n";
  }

  output << this->totalElements() << " 0\n";
  for (int i = 1; i <= this->totalElements(); i++) {
    const Element * element = this->element(i);
    for (int j = 1; j <= element->totalNodes(); j++) {
      output << element->node(j) << " ";
    }
    output << "\n";
  }
}
