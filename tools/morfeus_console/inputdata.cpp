#include "inputdata.h"

#include "geometry.h"
#include "materialdatabase.h"
#include "mesher.h"
#include "solution.h"

#include "rapidxml_print.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

InputData::InputData()
  : mCavityHeight(0.0)
  , mFreqIncrement(0.0)
  , mFreqStart(0.0)
  , mFreqStop(0.0)
  , mGeometry(new Geometry)
  , mMaterialDatabase(new MaterialDatabase)
  , mMesher(new Mesher)
  , mSolution(new Solution)
{
}

InputData::~InputData()
{
}

void InputData::print()
{
  mGeometry->print();
  mMesher->print();
  mMaterialDatabase->print();
  mSolution->print();
}

void InputData::readFromFile(const std::string & fileName)
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

  rapidxml::xml_node<> * geomNode = projectNode->first_node("Geometry", 0, false);
  if (geomNode == nullptr) {
    std::ostringstream oss;
    oss << "Error in input file: Could not find geometry specification";
    throw std::invalid_argument(oss.str());
  }
  mGeometry->readFromXml(document, geomNode);

  rapidxml::xml_node<> * materialsNode = projectNode->first_node("Materials", 0, false);
  if (materialsNode == nullptr) {
    std::ostringstream oss;
    oss << "Error in input file: Could not find materials specification";
    throw std::invalid_argument(oss.str());
  }
  mMaterialDatabase->readFromXml(document, materialsNode);

  rapidxml::xml_node<> * mesherNode = projectNode->first_node("Mesher", 0, false);
  if (mesherNode == nullptr) {
    std::ostringstream oss;
    oss << "Error in input file: Could not find mesher specification";
    throw std::invalid_argument(oss.str());
  }
  mMesher->readFromXml(document, mesherNode);

  rapidxml::xml_node<> * solutionNode = projectNode->first_node("Solution", 0, false);
  if (solutionNode == nullptr) {
    std::ostringstream oss;
    oss << "Error in input file: Could not find solution specification";
    throw std::invalid_argument(oss.str());
  }
  mSolution->readFromXml(document, solutionNode);
}

void InputData::saveToFile(const std::string &fileName)
{
  rapidxml::xml_document<> document;

  rapidxml::xml_node<> * xml_decl = document.allocate_node(rapidxml::node_declaration);
  xml_decl->append_attribute(document.allocate_attribute("version", "1.0"));
  xml_decl->append_attribute(document.allocate_attribute("encoding", "utf-8"));
  document.append_node(xml_decl);

  rapidxml::xml_node<> * projectNode = xmlutils::createNode(document, "Project");
  mGeometry->writeToXml(document, projectNode);
  mMesher->writeToXml(document, projectNode);
  mMaterialDatabase->writeToXml(document, projectNode);
  mSolution->writeToXml(document, projectNode);

  document.append_node(projectNode);

  std::ofstream output(fileName);
  output << document;
}

void InputData::validate() const
{

}
