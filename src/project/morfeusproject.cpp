#include "morfeusproject.h"

#include "geometry/model.h"
#include "materialdatabase.h"
#include "mesher.h"
#include "rapidxml_print.hpp"
#include "solution.h"

#include <fstream>

namespace morfeus {

MorfeusProject::MorfeusProject()
  : mModel(new geometry::Model)
  , mMaterialDatabase(new MaterialDatabase)
  , mMesher(new Mesher)
  , mSolution(new Solution)
{
}

void MorfeusProject::print(std::ostream & output, int tabPos) const
{
  mModel->print(output, tabPos);
  mMaterialDatabase->print(output, tabPos);
  mMesher->print(output, tabPos);
  mSolution->print(output, tabPos);
}

void MorfeusProject::print(int tabPos) const
{
  print(std::cout, tabPos);
}

void MorfeusProject::readFromFile(const std::string & fileName)
{
  std::ifstream input(fileName);
  if (input.fail()) {
    std::ostringstream oss;
    oss << "Could not open file for reading: " << fileName;
    throw std::invalid_argument(oss.str());
  }

  std::string fileContents((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
  fileContents.push_back('\0');

  rapidxml::xml_document<> document;
  try {
    document.parse<0>(&fileContents[0]);
  }
  catch (rapidxml::parse_error & ex) {
    std::cout << ex.what() << " " << ex.where<char>() << "\n";
    return;
  }

  rapidxml::xml_node<> * projectNode = document.first_node("Project", 0, false);
  if (projectNode == nullptr) {
    std::ostringstream oss;
    oss << "Error in input file: Could not find project specification";
    throw std::invalid_argument(oss.str());
  }
  readFromXml(document, projectNode);
}

void MorfeusProject::readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  rapidxml::xml_node<> * geomNode = node->first_node("Model");
  if (geomNode != nullptr) {
    mModel->readFromXml(document, geomNode);
  }

  rapidxml::xml_node<> * materialsNode = node->first_node("Materials");
  if (materialsNode != nullptr) {
    mMaterialDatabase->readFromXml(document, materialsNode);
  }

  rapidxml::xml_node<> * mesherNode = node->first_node("Mesher");
  if (mesherNode != nullptr) {
    mMesher->readFromXml(document, mesherNode);
  }

  rapidxml::xml_node<> * solutionNode = node->first_node("Solution");
  if (solutionNode != nullptr) {
    mSolution->readFromXml(document, solutionNode);
  }
}

void MorfeusProject::saveToFile(const std::string &fileName)
{
  rapidxml::xml_document<> document;

  rapidxml::xml_node<> * xml_decl = document.allocate_node(rapidxml::node_declaration);
  xml_decl->append_attribute(document.allocate_attribute("version", "1.0"));
  xml_decl->append_attribute(document.allocate_attribute("encoding", "utf-8"));
  document.append_node(xml_decl);

  rapidxml::xml_node<> * projectNode = xmlutils::createNode(document, "Project");
  writeToXml(document, projectNode);
  document.append_node(projectNode);

  std::ofstream output(fileName);
  output << document;
}

void MorfeusProject::validate() const
{

}

void MorfeusProject::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * geomNode = xmlutils::createNode(document, "Model");
  mModel->writeToXml(document, geomNode);

  rapidxml::xml_node<> * materialsNode = xmlutils::createNode(document, "Materials");
  mMaterialDatabase->writeToXml(document, materialsNode);

  rapidxml::xml_node<> * mesherNode = xmlutils::createNode(document, "Mesher");
  mMesher->writeToXml(document, mesherNode);

  rapidxml::xml_node<> * solutionNode = xmlutils::createNode(document, "Solution");
  mSolution->writeToXml(document, solutionNode);
}

}
