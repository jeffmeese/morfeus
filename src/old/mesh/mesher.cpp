#include "mesh/mesher.h"
#include "mesh/mesh.h"
#include "mesh/node.h"
#include "mesh/edge.h"
#include "mesh/element.h"
#include "mesh/prism.h"
#include "mesh/hexahedral.h"
#include "mesh/meshproperties.h"
#include "mesh/meshsection.h"
#include "mesh/face.h"
#include "mesh/quadrilateral.h"
#include "mesh/triangle.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <algorithm>
#include <memory>

//Mesher::Mesher(const InputData & inputData, Mesh & mesh)
//  : mInputData(inputData)
//  , mMesh(mesh)
//{
//}

//bool Mesher::importGrdFile(const std::string &filename)
//{
//  if (!readGrdFile(filename, mInputData, mMesh))
//    return false;

//  std::size_t surfaceElements = mMesh.totalElements();

//  if (!extrudeMesh(mInputData, mMesh))
//    return false;

//  if (!generateEdgeSigns(mMesh, surfaceElements))
//    return false;

//  if (!markUnknowns(mInputData, mMesh, surfaceElements))
//    return false;

//  if (!validateMesh(mMesh))
//    return false;

//  return true;
//}

//std::unique_ptr<Face> Mesher::createFace(std::size_t nodes, std::size_t edges)
//{
//  std::unique_ptr<Face> face;

//  if (nodes == 3 && edges == 3)
//    face.reset(new Triangle);
//  else if (nodes == 4 && edges == 4)
//    face.reset(new Quadrilateral);

//  return std::move(face);
//}

//bool Mesher::extrudeMesh(const InputData &inputData, Mesh &mesh)
//{
//  std::vector<Node*> surfaceNodes = mesh.getSurfaceNodes();
//  std::vector<Element*> surfaceElements;
//  for (std::size_t i = 1; i <= mesh.totalElements(); i++)
//    surfaceElements.push_back(mesh.getElement(i));

//  for (std::size_t i = 1; i < inputData.totalLayers(); i++) {
//    double layerHeight = inputData.layerHeight(i);

//    std::size_t start_node = 1 + i*surfaceNodes.size();
//    std::size_t end_node = start_node + surfaceNodes.size() - 1;
//    std::size_t start_element = 1 + (i-1)*surfaceElements.size();
//    std::size_t end_element = start_element + surfaceElements.size() - 1;

//    for (std::size_t j = start_node; j <= end_node; j++) {
//      Node * node = mesh.getNode(j);
//      std::size_t id = mesh.totalNodes() + 1;
//      auto newNode(std::make_unique<Node>(id, node->x(), node->y(), node->z()-layerHeight));
//      mesh.addNode(std::move(newNode));
//    }

//    for (std::size_t j = start_element; j <= end_element; j++) {
//      Element * element = mesh.getElement(j);
//      std::size_t id = mesh.totalElements() + 1;

//      // Copy the element nodes
//      std::size_t topNodes = element->totalNodes() / 2;
//      auto newElement = element->copy(id);
//      for (std::size_t k = 1; k <= topNodes; k++) {
//        newElement->setGlobalNode(k, element->globalNode(k+topNodes));
//        newElement->setGlobalNode(k+topNodes, newElement->globalNode(k) + surfaceNodes.size());
//      }

//      // Create the element edges
//      generateElementEdges(newElement.get(), mesh);

//      // Add the mesh element
//      mesh.addElement(std::move(newElement));
//    }
//  }

//  return true;
//}

//bool Mesher::generateEdgeSigns(Mesh & mesh, std::size_t apertureElements)
//{
//  // Assume all edges start with a sign of 1
//  for (std::size_t i = 1; i <= mesh.totalElements(); i++) {
//    Element * element = mesh.getElement(i);
//    for (std::size_t j = 1; j <= element->totalEdges(); j++) {
//      element->setEdgeSign(j, 1);
//    }
//  }

//  // Determine the signs for each top and bottom face for each
//  // element layer by layer. If there were only one type of element
//  // this would be easier since we could just copy the signs
//  // from element to element but we have multiple types of elements.
//  std::size_t totalLayers = mesh.totalElements() / apertureElements;
//  for (std::size_t m = 1; m <= totalLayers; m++) {
//    std::size_t startElement = (m-1)*apertureElements + 1;
//    std::size_t endElement = startElement + apertureElements - 1;

//    std::set<std::size_t> topEdgeSet, bottomEdgeSet;
//    for (std::size_t i = startElement; i <= endElement; i++) {
//      Element * element = mesh.getElement(i);

//      for (std::size_t j = 1; j <= element->totalFaces(); j++) {
//        std::unique_ptr<Face> face = element->createFace(j);

//        // Identify the signs on top faces
//        if (face->position() == Face::Position::Top) {
//          for (std::size_t k = 1; k <= face->totalEdges(); k++) {
//            std::size_t globalEdge = face->globalEdge(k);
//            if (topEdgeSet.find(globalEdge) != topEdgeSet.end()) {
//              for (std::size_t n = 1; n <= element->totalEdges(); n++) {
//                if (element->globalEdge(n) == globalEdge)
//                  element->setEdgeSign(n,-1);
//              }
//            }
//            topEdgeSet.insert(globalEdge);
//          }
//        }

//        // Identify the signs on bottom faces
//        if (face->position() == Face::Position::Bottom) {
//          for (std::size_t k = 1; k <= face->totalEdges(); k++) {
//            std::size_t globalEdge = face->globalEdge(k);
//            if (bottomEdgeSet.find(globalEdge) != bottomEdgeSet.end()) {
//              for (std::size_t n = 1; n <= element->totalEdges(); n++) {
//                if (element->globalEdge(n) == globalEdge)
//                  element->setEdgeSign(n,-1);
//              }
//            }
//            bottomEdgeSet.insert(globalEdge);
//          }
//        }
//      }
//    }
//  }

//  return true;
//}

//void Mesher::generateElementEdges(Element * element, Mesh & mesh)
//{
//	for (std::size_t i = 1; i <= element->totalEdges(); i++) {
//		std::size_t localNode1 = 0, localNode2 = 0;
//		element->getEdgeNodes(i, localNode1, localNode2);

//		std::size_t globalNode1 = element->globalNode(localNode1);
//		std::size_t globalNode2 = element->globalNode(localNode2);
//		std::size_t globalEdge = mesh.findEdge(globalNode1, globalNode2);
//		if (globalEdge == 0) {
//			globalEdge = mesh.totalEdges() + 1;
//			auto newEdge(std::make_unique<Edge>(globalEdge, globalNode1, globalNode2));
//			mesh.addEdge(std::move(newEdge));
//		}
//		element->setGlobalEdge(i, globalEdge);
//	}
//}

//bool Mesher::markUnknowns(const InputData &inputData, Mesh &mesh, std::size_t totalApertureElements)
//{
//  // Initialize all edges to be unknown
//  for (std::size_t i = 1; i <= mesh.totalEdges(); i++) {
//    mesh.getEdge(i)->setType(Edge::Type::Dielectric);
//  }

//  // Use the section information to mark antenna and metal elements and edges
//  for (std::size_t i = 0; i < inputData.totalMeshSections(); i++) {
//    MeshSection * meshSection = inputData.meshSection(i);
//    std::size_t elementCode = meshSection->elementCode();
//    std::size_t sectionLayer = meshSection->layer();
//    std::size_t epsId = meshSection->epsId();
//    std::size_t muId = meshSection->muId();
//    Element::Type elementType = Element::Type(meshSection->elementType());
//    std::size_t startElement = 1 + (sectionLayer-1)*totalApertureElements;
//    std::size_t endElement = startElement + totalApertureElements - 1;

//    // Only handle non-dielectric elements
//    for (std::size_t j = startElement; j <= endElement; j++) {
//      Element * element = mesh.getElement(j);
//      element->setEpsId(epsId);
//      element->setMuId(muId);

//      if (elementType != Element::Type::Dielectric) {

//        if (element->section() == elementCode) {
//          // Antenna elements only have metal edges on the top face
//          if (elementType == Element::Type::Antenna) {
//            for (std::size_t k = 1; k <= element->totalFaces(); k++) {
//              std::unique_ptr<Face> face = element->createFace(k);
//              if (face->position() == Face::Position::Top) {
//                for (std::size_t m = 1; m <= face->totalEdges(); m++) {
//                  Edge * edge = mesh.getEdge(face->globalEdge(m));
//                  edge->setType(Edge::Type::Metal);
//                }
//              }
//            }
//          }

//          // Metal elements have all metal edges
//          if (elementType == Element::Type::Metal) {
//            std::size_t totalEdges = element->totalEdges();
//            for (std::size_t k = 1; k <= totalEdges; k++) {
//              std::size_t globalEdge = element->globalEdge(k);
//              Edge * meshEdge = mesh.getEdge(globalEdge);
//              meshEdge->setType(Edge::Type::Metal);
//            }
//          }
//        }
//      }
//    }
//  }

//  // Get the boundary faces from the mesh
//  std::vector<std::unique_ptr<Face>> boundaryFaces = mesh.getBoundaryFaces();

//  // Mark all edges of non-top boundary faces as boundary edges
//  for (std::size_t i = 0; i < boundaryFaces.size(); i++) {
//    Face * face = boundaryFaces.at(i).get();
//    if (face->position() != Face::Position::Top) {
//      for (std::size_t j = 1; j <= face->totalEdges(); j++) {
//        Edge * edge = mesh.getEdge(face->globalEdge(j));
//        if (edge != nullptr)
//          edge->setType(Edge::Type::Boundary);
//      }
//    }
//  }

//  // Find the apeture edges so we can number the unknowns there first
//  MeshProperties meshProperties(mesh);
//  const std::vector<Edge*> apertureEdges = meshProperties.apertureEdges();

//  // Number the aperture edge unknowns
//  int unknownNumber = 0;
//  for (std::size_t i = 0; i < apertureEdges.size(); i++) {
//    Edge * edge = apertureEdges.at(i);
//    if (edge->type() == Edge::Type::Dielectric)
//      edge->setUnknown(++unknownNumber);
//  }

//  // Number the rest of the unknowns
//  for (std::size_t i = 1; i <= mesh.totalEdges(); i++) {
//    Edge * edge = mesh.getEdge(i);
//    if (edge->type() == Edge::Type::Dielectric && edge->unknown() == 0)
//      edge->setUnknown(++unknownNumber);
//  }

//  return true;
//}

//bool Mesher::readGrdFile(const std::string & fileName, const InputData & inputData, Mesh & mesh)
//{
//	std::ifstream input(fileName.c_str());
//	if (input.fail()) {
//		std::cerr << "Could not open surface file " << fileName << "\n";
//		return false;
//	}

//	std::vector<std::unique_ptr<Face>> faceList;

//	std::size_t totalElements = 0, totalNodes = 0, totalSections = 0, temp = 0;
//	std::string line;
//	std::getline(input, line);
//	std::istringstream iss_params(line);
//	iss_params >> temp >> totalElements >> totalNodes >> temp >> temp >> totalSections >> temp;

//	// Skip the elements
//	for (std::size_t i = 1; i <= totalElements; i++) {
//		std::getline(input, line);
//		std::getline(input, line);
//	}

//	// Read the nodes
//	for (std::size_t i = 1; i <= totalNodes; i++) {
//		double x, y, z;
//		std::getline(input, line);
//		std::istringstream iss(line);
//		iss >> x >> y >> z;

//		auto node(std::make_unique<Node>(i, x, y, z));
//		mesh.addNode(std::move(node));
//	}

//	// Create the surface nodes
//	std::size_t surfaceNodes = mesh.totalNodes();
//	double layerHeight = inputData.layerHeight(0);
//	for (std::size_t i = 1; i <= surfaceNodes; i++) {
//		Node * node = mesh.getNode(i);

//		auto newNode(std::make_unique<Node>(node->id() + surfaceNodes));
//		newNode->move(node->x(), node->y(), node->z()-layerHeight);
//		mesh.addNode(std::move(newNode));
//	}

//	// Go back to the beginning of the file to read the elements
//	input.clear();
//	input.seekg(0);

//	// Skip the header line
//	std::getline(input, line);

//	// Read the elements
//	for (size_t i = 1; i <= totalElements; i++) {
//		std::size_t elementNodes = 0, elementEdges = 0, section = 0;
//		std::getline(input, line);
//		std::istringstream iss(line);
//		iss >> elementNodes >> elementEdges >> section;

//		// Create the new element
//		auto face(createFace(elementNodes, elementEdges));
//		if (face == nullptr) {
//			std::cerr << "Unable to create face with " << elementNodes << " nodes and " << elementEdges << " edges\n";
//			return false;
//		}

//		face->setSectionId(section);
//		if (section == 0) {
//			std::cerr << "***Error: Element " << i << " has a section number of 0\n";
//			return false;
//		}

//		// Set the element surface node numbers
//		std::getline(input, line);
//		std::istringstream iss_nodes(line);
//		for (std::size_t j = 1; j <= elementNodes; j++) {
//			std::size_t node_number = 0;
//			iss_nodes >> node_number;
//			face->setGlobalNode(j, node_number);
//		}

//		faceList.push_back(std::move(face));
//	}

//	// Create the elements from the list of faces
//	for (std::size_t i = 0; i < faceList.size(); i++) {
//		Face * face = faceList.at(i).get();
//		std::unique_ptr<Element> element = face->extrudeElement(i+1, totalNodes);
//		element->setSection(face->sectionId());
//		generateElementEdges(element.get(), mesh);
//		mesh.addElement(std::move(element));
//	}

//	return true;
//}

//bool Mesher::validateMesh(const Mesh & mesh)
//{
//	return true;
//}
