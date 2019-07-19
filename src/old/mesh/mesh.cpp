#include "mesh/mesh.h"
#include "mesh/node.h"
#include "mesh/edge.h"
#include "mesh/element.h"
#include "mesh/face.h"
#include "mesh/triangle.h"

#include "material/material.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

Mesh::Mesh()
{
	mMinX = mMinY = mMinZ = mMaxX = mMaxY = mMaxZ = 0.0;
	mNodeTolerance = 1e-6;
}

void Mesh::addEdge(std::unique_ptr<Edge> edge)
{
	std::size_t globalNode1 = edge->globalNode1();
	std::size_t globalNode2 = edge->globalNode2();

	EdgePair ep1 = {globalNode2, edge->id()};
	EdgePair ep2 = {globalNode2, edge->id()};

	mEdgeTable[globalNode1].push_back(ep1);
	mEdgeTable[globalNode2].push_back(ep2);

	mEdges.push_back(std::move(edge));
}

void Mesh::addElement(std::unique_ptr<Element> element)
{
	mElements.push_back(std::move(element));
}

void Mesh::addMaterial(std::unique_ptr<Material> material)
{
	mMaterials.push_back(std::move(material));
}

void Mesh::addNode(std::unique_ptr<Node> node)
{
	if (mNodes.size() == 0 || node->x() < mMinX)
		mMinX = node->x();

	if (mNodes.size() == 0 || node->y() < mMinY)
		mMinY = node->y();

	if (mNodes.size() == 0 || node->z() < mMinZ)
		mMinZ = node->z();

	if (mNodes.size() == 0 || node->x() > mMaxX)
		mMaxX = node->x();

	if (mNodes.size() == 0 || node->y() > mMaxY)
		mMaxY = node->y();

	if (mNodes.size() == 0 || node->z() > mMaxZ)
		mMaxZ = node->z();

	mNodes.push_back(std::move(node));
}

Edge * Mesh::findEdge(std::size_t node1, std::size_t node2) const
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

Material * Mesh::findMaterial(std::size_t id) const
{
  for (std::size_t i = 0; i < mMaterials.size(); i++) {
    if (mMaterials.at(i)->id() == id)
      return mMaterials.at(i).get();
  }
  return nullptr;
}

std::size_t Mesh::findNearestNode(double x, double y, double z) const
{
	std::size_t nearestNode = 0;

	double minR = 0.0;
	for (std::size_t i = 0; i < totalNodes(); i++) {

		const Node * node = getNode(i);
		double x2 = node->x(), y2 = node->y(), z2 = node->z();
		double R = sqrt( (x-x2)*(x-x2) + (y-y2)*(y-y2) + (z-z2)*(z-z2));
		if (R <= mNodeTolerance)
			return node->id();

		if (i == 1 || R < minR) {
			nearestNode = node->id();
			minR = R;
		}
	}

	return nearestNode;
}

std::vector<std::unique_ptr<Face>> Mesh::getBoundaryFaces() const
{
  // Make a list of faces for all elements
  using FaceList = std::vector<std::unique_ptr<Face>>;
  std::map<std::size_t, FaceList> faceTable;
  std::set<size_t> mapKeys;

  // Determine the boundary faces
  for (std::size_t i = 0; i < totalElements(); i++) {
    Element * element = getElement(i);
    std::size_t totalFaces = element->totalFaces();
    for (std::size_t j = 0; j < totalFaces; j++) {
      std::unique_ptr<Face> face = element->createFace(j);

      // Sum the global nodes to use as a key
      std::size_t sum = 0;
      for (std::size_t k = 0; k < face->totalNodes(); k++)
        sum += face->globalNode(k);

      // I really wish std::map had a keys() function
      // but until it does I'll just track of the keys
      // in a set
      mapKeys.insert(sum);

      // Determine if a coincident face is already in the list
      bool matchFound = false;
      FaceList & faceList = faceTable[sum];
      for (std::size_t k = 0; k < faceList.size(); k++) {
        if (face->isCoincident(faceList.at(k).get())) {
          faceList.erase(faceList.begin() + k);
          matchFound = true;
          break;
        }
      }

      // If no coincident face was found add it to the list
      if (!matchFound)
        faceTable[sum].push_back(std::move(face));
    }
  }

  // Populate the boundary face vector from the table
  using set_iterator = std::set<std::size_t>::const_iterator;
  std::vector<std::unique_ptr<Face>> boundaryFaces;
  for (set_iterator itr = mapKeys.begin(); itr != mapKeys.end(); ++itr) {
    std::size_t key(*itr);
    FaceList & faceList = faceTable[key];
    for (std::size_t i = 0; i < faceList.size(); i++)
      boundaryFaces.push_back(std::move(faceList.at(i)));
  }

  return boundaryFaces;
}

Edge * Mesh::getEdge(std::size_t index) const
{
	if (index >= mEdges.size())
		std::cerr << "Invalid index in getEdge\n";

	return mEdges.at(index).get();
}

Element * Mesh::getElement(std::size_t index) const
{
	if (index >= mElements.size())
		std::cerr << "Invalid index in getElement\n";

	return mElements.at(index).get();
}

Material * Mesh::getMaterial(std::size_t index) const
{
	if (index >= mMaterials.size())
		std::cerr << "Invalid index in getMaterial\n";

	return mMaterials.at(index).get();
}

Node * Mesh::getNode(std::size_t index) const
{
	if (index >= mNodes.size()) {
		std::cerr << "Invalid index in getNode\n";
		std::cerr << index << " " << mNodes.size() << "\n";
	}

	return mNodes.at(index).get();
}

std::vector<Element*> Mesh::getSurfaceElements() const
{
	std::vector<Element*> surfaceElements;
	return surfaceElements;
}

std::vector<Node*> Mesh::getSurfaceNodes() const
{
	std::vector<Node*> surfaceNodes;
	for (std::size_t i = 0; i < totalNodes(); i++) {
		Node * node = getNode(i);
		if (node->z() >= 0.0)
			surfaceNodes.push_back(node);
	}
	return surfaceNodes;
}

//std::vector<std::unique_ptr<Face>> Mesh::importGrdFile(const std::string & fileName)
//{
//	std::vector<std::unique_ptr<Face>> faceList;

//	std::ifstream input(fileName.c_str());
//	if (input.fail()) {
//		std::cerr << "Could not open surface file " << fileName << "\n";
//		return faceList;
//	}

//	//double scaleFactor = inputData.scaleFactor();
//	std::size_t totalElements = 0, totalNodes = 0, totalSections = 0, temp = 0;
//	std::string line;
//	std::getline(input, line);
//	std::istringstream iss_params(line);
//	iss_params >> temp >> totalElements >> totalNodes >> temp >> temp >> totalSections >> temp;

//	// Skip the elements
//	for (std::size_t i = 0; i < totalElements; i++) {
//		std::getline(input, line);
//		std::getline(input, line);
//	}

//	// Read the nodes
//	for (std::size_t i = 0; i < totalNodes; i++) {
//		double x, y, z;
//		std::getline(input, line);
//		std::istringstream iss(line);
//		iss >> x >> y >> z;

//		auto node(std::make_unique<Node>(i, x, y, z));
//		addNode(std::move(node));
//	}

//	// Go back to the beginning of the file to read the elements
//	input.clear();
//	input.seekg(0);

//	// Skip the header line
//	std::getline(input, line);

//	// Read the elements
//	for (size_t i = 0; i < totalElements; i++) {
//		std::size_t elementNodes = 0, elementEdges = 0, section = 0;
//		std::getline(input, line);
//		std::istringstream iss(line);
//		iss >> elementNodes >> elementEdges >> section;

//		// Create the new element
//		//auto face(createFace(elementNodes, elementEdges));
//		std::unique_ptr<Face> face(new Triangle);
//		if (face == nullptr) {
//			std::cerr << "Unable to create face with " << elementNodes << " nodes and " << elementEdges << " edges\n";
//			return faceList;
//		}

//		face->setSectionId(section);
//		if (section == 0) {
//			std::cerr << "***Error: Element " << i << " has a section number of 0\n";
//			return faceList;
//		}

//		// Set the element surface node numbers
//		std::getline(input, line);
//		std::istringstream iss_nodes(line);
//		for (std::size_t j = 0; j < elementNodes; j++) {
//			std::size_t node_number = 0;
//			iss_nodes >> node_number;
//			face->setGlobalNode(j, node_number-1);
//		}

//		faceList.push_back(std::move(face));
//	}

//	return faceList;
//}

double Mesh::maxX() const
{
	return mMaxX;
}

double Mesh::maxY() const
{
	return mMaxY;
}

double Mesh::maxZ() const
{
	return mMaxZ;
}

double Mesh::minX() const
{
	return mMinX;
}

double Mesh::minY() const
{
	return mMinY;
}

double Mesh::minZ() const
{
	return mMinZ;
}

std::size_t Mesh::totalEdges() const
{
  return mEdges.size();
}

std::size_t Mesh::totalElements() const
{
  return mElements.size();
}

std::size_t Mesh::totalMaterials() const
{
  return mMaterials.size();
}

std::size_t Mesh::totalNodes() const
{
  return mNodes.size();
}
