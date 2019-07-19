#include "surfacemesh.h"
#include "triangle.h"
#include "mesh.h"

#include <fstream>
#include <sstream>
#include <iostream>

SurfaceMesh::SurfaceMesh()
{
}

SurfaceMesh::SurfaceMesh(const std::string & name)
	: mName(name)
{
}

void SurfaceMesh::addEdge(std::unique_ptr<Edge> edge)
{
	mEdges.push_back(std::move(edge));
}

void SurfaceMesh::addFace(std::unique_ptr<Face> face)
{
	mFaces.push_back(std::move(face));
}

void SurfaceMesh::addNode(std::unique_ptr<Node> node)
{
	mNodes.push_back(std::move(node));
}

void SurfaceMesh::generateMesh(Mesh & mesh, double layerHeight) const
{
	// Create the bottom nodes of the first layer nodes
	std::size_t surfaceNodes = totalNodes();
	for (std::size_t i = 0; i < surfaceNodes; i++) {
		Node * node = getNode(i);

		auto surfaceNode(std::make_unique<Node>(node->id()));
		surfaceNode->move(node->x(), node->y(), node->z());
		mesh.addNode(std::move(surfaceNode));
	}

	for (std::size_t i = 0; i < surfaceNodes; i++) {
		Node * node = getNode(i);

		auto newNode(std::make_unique<Node>(node->id() + surfaceNodes));
		newNode->move(node->x(), node->y(), node->z()-layerHeight);
		mesh.addNode(std::move(newNode));
	}

	// Create the elements from the list of faces
	std::size_t faces = totalFaces();
	for (std::size_t i = 0; i < faces; i++) {
		Face * face = getFace(i);
		std::unique_ptr<Element> element = face->extrudeElement(i, surfaceNodes);
		element->setSection(face->sectionId());
		generateElementEdges(element.get(), mesh);
		mesh.addElement(std::move(element));
	}
}

void SurfaceMesh::generateElementEdges(Element * element, Mesh & mesh) const
{
	for (std::size_t i = 0; i < element->totalEdges(); i++) {
		std::size_t localNode1 = 0, localNode2 = 0;
		element->getEdgeNodes(i, localNode1, localNode2);

		std::size_t globalNode1 = element->globalNode(localNode1);
		std::size_t globalNode2 = element->globalNode(localNode2);
		Edge * globalEdge = mesh.findEdge(globalNode1, globalNode2);
		if (globalEdge == nullptr) {
			std::size_t edgeNumber = mesh.totalEdges();
			auto newEdge(std::make_unique<Edge>(edgeNumber, globalNode1, globalNode2));
			mesh.addEdge(std::move(newEdge));
			globalEdge = mesh.getEdge(edgeNumber);
		}
		element->setGlobalEdge(i, globalEdge->id());
	}
}

Edge * SurfaceMesh::getEdge(std::size_t index) const
{
	return mEdges.at(index).get();
}

Face * SurfaceMesh::getFace(std::size_t index) const
{
	return mFaces.at(index).get();
}

Node * SurfaceMesh::getNode(std::size_t index) const
{
	return mNodes.at(index).get();
}

bool SurfaceMesh::importGrdFile(const std::string & fileName)
{
	mNodes.clear();
	mEdges.clear();
	mFaces.clear();

	std::ifstream input(fileName.c_str());
	if (input.fail()) {
		std::cerr << "Could not open surface file " << fileName << "\n";
		return false;
	}

	//double scaleFactor = inputData.scaleFactor();
	std::size_t totalElements = 0, totalNodes = 0, totalSections = 0, temp = 0;
	std::string line;
	std::getline(input, line);
	std::istringstream iss_params(line);
	iss_params >> temp >> totalElements >> totalNodes >> temp >> temp >> totalSections >> temp;

	// Skip the elements
	for (std::size_t i = 0; i < totalElements; i++) {
		std::getline(input, line);
		std::getline(input, line);
	}

	// Read the nodes
	for (std::size_t i = 0; i < totalNodes; i++) {
		double x, y, z;
		std::getline(input, line);
		std::istringstream iss(line);
		iss >> x >> y >> z;

		auto node(std::make_unique<Node>(i, x, y, z));
		addNode(std::move(node));
	}

	// Go back to the beginning of the file to read the elements
	input.clear();
	input.seekg(0);

	// Skip the header line
	std::getline(input, line);

	// Read the elements
	for (size_t i = 0; i < totalElements; i++) {
		std::size_t elementNodes = 0, elementEdges = 0, section = 0;
		std::getline(input, line);
		std::istringstream iss(line);
		iss >> elementNodes >> elementEdges >> section;

		// Create the new element
		//auto face(createFace(elementNodes, elementEdges));
		std::unique_ptr<Face> face(new Triangle);
		if (face == nullptr) {
			std::cerr << "Unable to create face with " << elementNodes << " nodes and " << elementEdges << " edges\n";
			return false;
		}

		face->setSectionId(section);
		if (section == 0) {
			std::cerr << "***Error: Face " << i << " has a section number of 0\n";
			return false;
		}

		// Set the element surface node numbers
		std::getline(input, line);
		std::istringstream iss_nodes(line);
		for (std::size_t j = 0; j < elementNodes; j++) {
			std::size_t node_number = 0;
			iss_nodes >> node_number;
			face->setGlobalNode(j, node_number-1);
		}

		mFaces.push_back(std::move(face));
	}

	return true;
}

bool SurfaceMesh::importGrdMeshFile(const std::string & fileName)
{
	mNodes.clear();
	mEdges.clear();
	mFaces.clear();

	std::ifstream input(fileName.c_str());
	if (input.fail()) {
		std::cerr << "Could not open surface file " << fileName << std::endl;
		return false;
	}

	while (true) {
		std::string inputLine;
		std::getline(input, inputLine);
		if (inputLine == "$Nodes") {
			std::size_t totalNodes = 0;
			std::getline(input, inputLine);
			std::istringstream iss(inputLine);
			iss >> totalNodes;
			for (std::size_t i = 0; i < totalNodes; i++) {
				std::size_t nodeNumber = 0;
				double x = 0.0, y = 0.0, z = 0.0;
				std::getline(input, inputLine);
				std::istringstream iss(inputLine);
				iss >> nodeNumber >> x >> y >> z;

				std::unique_ptr<Node> newNode = std::make_unique<Node>(i, x, y, z);
				addNode(std::move(newNode));
			}
			std::getline(input, inputLine);
		}

		if (inputLine == "$Elements") {
			std::size_t totalElements = 0;
			std::getline(input, inputLine);
			std::istringstream iss(inputLine);
			iss >> totalElements;

			std::size_t elementsRead = 0;
			for (std::size_t i = 0; i < totalElements; i++) {
				std::size_t elemNumber = 0, elementType = 0, elementSection = 0, numTags = 0;
				std::getline(input, inputLine);
				std::istringstream iss(inputLine);
				iss >> elemNumber >> elementType >> numTags;

				for (std::size_t j = 0; j < numTags; j++) {
					std::size_t tag;
					iss >> tag;
					if (j == 0)
						elementSection = tag;
				}

				if (elementType == 2) {
					++elementsRead;
					std::unique_ptr<Face> newFace(new Triangle);
					if (elementSection == 0)
						elementSection = 1;

					newFace->setSectionId(elementSection);
					for (std::size_t j = 0; j < 3; j++) {
						std::size_t nodeNumber = 0;
						iss >> nodeNumber;
						newFace->setGlobalNode(j, nodeNumber-1);
					}
					addFace(std::move(newFace));
				}
			}
			break;
		}
	}

	return true;
}

std::string SurfaceMesh::name() const
{
	return mName;
}

void SurfaceMesh::setName(const std::string &name)
{
	mName = name;
}

std::size_t SurfaceMesh::totalEdges() const
{
	return mEdges.size();
}

std::size_t SurfaceMesh::totalFaces() const
{
	return mFaces.size();
}

std::size_t SurfaceMesh::totalNodes() const
{
	return mNodes.size();
}
