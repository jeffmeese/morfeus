#ifndef MESHER_H
#define MESHER_H

#include "morfeus.h"

#include <string>
#include <memory>

class Mesh;
class Face;
class Element;
class InputData;

//class MORFEUS_LIB_DECL Mesher
//{
//public:
//  Mesher(const InputData & inputData, Mesh & mesh);

//public:
//  bool importGrdFile(const std::string & filename);

//private:
//  std::unique_ptr<Face> createFace(std::size_t nodes, std::size_t edges);
//  bool extrudeMesh(const InputData & inputData, Mesh & mesh);
//  void generateElementEdges(Element * element, Mesh & mesh);
//  bool generateEdgeSigns(Mesh & mesh, std::size_t apertureElements);
//  bool markUnknowns(const InputData & inputData, Mesh & mesh, std::size_t totalApertureElements);
//  bool readGrdFile(const std::string &fileName, const InputData & inputData, Mesh & mesh);
//  bool validateMesh(const Mesh & mesh);

//private:
//  const InputData & mInputData;
//  Mesh & mMesh;
//};

#endif // MESHER_H
