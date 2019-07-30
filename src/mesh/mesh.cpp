#include "mesh.h"
#include "edge.h"
#include "element.h"
#include "material.h"
#include "node.h"

#include <fstream>
#include <sstream>

namespace Morfeus {
namespace mesh {

Mesh::Mesh()
  : MorfeusObject("Mesh")
  , mMaterialDatabase(nullptr)
{
  mNodeTolerance = 1e-6;
}

Mesh::~Mesh()
{
}

void Mesh::addEdge(std::unique_ptr<Edge> edge)
{
  mEdgeTable[edge->node1()].push_back(EdgePair(edge->node2(), edge->number()));
  mEdgeTable[edge->node2()].push_back(EdgePair(edge->node1(), edge->number()));
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
    element->edgeNodes(i, edgeNode1, edgeNode2);
    int node1 = element->node(edgeNode1);
    int node2 = element->node(edgeNode2);
    int edgeNumber = -1;
    Edge * edge = findEdge(node1, node2);
    if (edge != nullptr) {
      edgeNumber = edge->number();
    }
    else {
      edgeNumber = static_cast<int32_t>(mEdges.size());
      EdgePtr edge(new Edge(edgeNumber, node1, node2));
      addEdge(std::move(edge));
    }
    element->setEdge(i, edgeNumber);


//    int edgeNumber = -1;
//    EdgeList & edgeList1 = mEdgeTable[node1];
//    for (EdgeList::iterator itr = edgeList1.begin(); itr != edgeList1.end(); ++itr) {
//      if (itr->first == node2) {
//        edgeNumber = itr->second;
//        break;
//      }
//    }

//    EdgeList & edgeList2 = mEdgeTable[node2];
//    for (EdgeList::iterator itr = edgeList2.begin(); itr != edgeList2.end(); ++itr) {
//      if (itr->first == node1) {
//        edgeNumber = itr->second;
//        break;
//      }
//    }

//    if (edgeNumber == -1) {
//      edgeNumber = static_cast<int32_t>(mEdges.size());
//      EdgePtr edge(new Edge(edgeNumber, node1, node2));
//      addEdge(std::move(edge));
//    }

  }
}

Edge * Mesh::edge(std::size_t index)
{
  return mEdges.at(index).get();
}

const Edge * Mesh::edge(std::size_t index) const
{
  return mEdges.at(index).get();
}

Element * Mesh::element(std::size_t index)
{
  return mElements.at(index).get();
}

const Element * Mesh::element(std::size_t index) const
{
  return mElements.at(index).get();
}

Edge * Mesh::findEdge(int32_t node1, int32_t node2) const
{
	typedef EdgeList::const_iterator Itr;
	typedef EdgeTable::const_iterator MapItr;

	// Look in node 1 list
	MapItr itr1 = mEdgeTable.find(node1);
	if (itr1 != mEdgeTable.end())
	{
		const std::list<EdgePair> & l1 = itr1->second;
		for (Itr itr = l1.begin(); itr != l1.end(); ++itr) {
			if (itr->first == node2) {
				return mEdges.at(itr->second).get();
			}
		}
	}

	// Look in node 2 list
	MapItr itr2 = mEdgeTable.find(node2);
	if (itr2 != mEdgeTable.end())
	{
		const std::list<EdgePair>& l2 = itr2->second;
		for (Itr itr = l2.begin(); itr != l2.end(); ++itr) {
			if (itr->first == node1) {
				return mEdges.at(itr->second).get();
			}
		}
	}

	return nullptr;
}

int32_t Mesh::findNearestNode(double x, double y, double z) const
{
	int32_t nearestNode = -1;

	double minR = 0.0;
	for (std::size_t i = 0; i < totalNodes(); i++) {

		const Node * node = mNodes.at(i).get();
		double x2 = node->x(), y2 = node->y(), z2 = node->z();
		double R = sqrt( (x-x2)*(x-x2) + (y-y2)*(y-y2) + (z-z2)*(z-z2));
		if (R <= mNodeTolerance)
			return node->number();

		if (i == 1 || R < minR) {
			nearestNode = node->number();
			minR = R;
		}
	}

	return nearestNode;
}

Node * Mesh::node(std::size_t index)
{
  return mNodes.at(index).get();
}

const Node * Mesh::node(std::size_t index) const
{
  return mNodes.at(index).get();
}

std::unique_ptr<Edge> Mesh::takeEdge(std::size_t index)
{
  std::unique_ptr<Edge> edge(mEdges.at(index).get());
  mEdges.erase(mEdges.begin() + index);
  return edge;
}

std::unique_ptr<Element> Mesh::takeElement(std::size_t index)
{
  std::unique_ptr<Element> element(mElements.at(index).get());
  mElements.erase(mElements.begin() + index);
  return element;
}

std::unique_ptr<Node> Mesh::takeNode(std::size_t index)
{
  std::unique_ptr<Node> node(mNodes.at(index).get());
  mNodes.erase(mNodes.begin() + index);
  return node;
}

std::size_t Mesh::totalEdges() const
{
  return mEdges.size();
}

std::size_t Mesh::totalElements() const
{
  return mElements.size();
}

std::size_t Mesh::totalNodes() const
{
  return mNodes.size();
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
  for (int i = 0; i < this->totalNodes(); i++) {
    const Node * node = this->node(i);
    output << node->x() << " " << node->y() << " " << node->z() << "\n";
  }

  output << this->totalElements() << " 0\n";
  for (int i = 0; i < this->totalElements(); i++) {
    const Element * element = this->element(i);
    for (int j = 0; j < element->totalNodes(); j++) {
      output << element->node(j) << " ";
    }
    output << "\n";
  }
}

void Mesh::writeVtkFile(const std::string &fileName) const
{
  std::ofstream output(fileName);

  output << "# vtk DataFile Version 2.0\n";
  output << "Unstructured Grid\n";
  output << "ASCII\n";
  output << "DATASET UNSTRUCTURED_GRID\n";
  output << "POINTS " << totalNodes() << " double\n";
  for (std::size_t i = 0; i < mNodes.size(); i++) {
    const Node * node = mNodes.at(i).get();
    output << node->x() << " " << node->y() << " " << node->z() << "\n";
  }
  output << "\n";

  output << "CELLS " << mElements.size() << " " << mElements.size()*5 << "\n";
  for (std::size_t i = 0; i < mElements.size(); i++) {
    const Element * element = mElements.at(i).get();
    output << element->totalNodes() << " ";
    for (std::size_t j = 0; j < element->totalNodes(); j++) {
      int32_t number = element->node(j);
      output << number << " ";
    }
    output << "\n";
  }
  output << "\n";

  output << "CELL_TYPES " << mElements.size() << "\n";
  for (std::size_t i = 0; i < mElements.size(); i++) {
    output << "10\n";
  }
  output << "\n";

  output << "CELL_DATA " << mElements.size() << "\n";
  output << "SCALARS cell_scalars int 1\n";
  output << "LOOKUP_TABLE default\n";
  for (std::size_t i = 0; i < mElements.size(); i++) {
    const Element * element = mElements.at(i).get();
    output << element->attribute() << "\n";
  }
  output << "\n";
}

}
}
