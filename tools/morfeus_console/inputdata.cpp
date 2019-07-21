#include "inputdata.h"

#include "geometry.h"
#include "materialdatabase.h"
#include "mesher.h"
#include "solution.h"

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

void InputData::readFromFile(const std::string & fileName)
{
  std::ifstream input(fileName);
  if (input.fail()) {
    std::ostringstream oss;
    oss << "Could not open file for reading: " << fileName;
    throw std::invalid_argument(oss.str());
  }

  std::ostringstream oss;
  std::string line;
  while (std::getline(input, line)) {
    if (input.fail()) {
      break;
    }
    oss << line;
  }

  std::string text(oss.str());
  char * parseText = new char[text.length()];
  strcpy(parseText, text.c_str());

  rapidxml::xml_document<> document;
  document.parse<0>(parseText);

  rapidxml::xml_node<> * geomNode = document.first_node("Geometry", 0, false);
  if (geomNode == nullptr) {
    std::ostringstream oss;
    oss << "Error in input file: Could not find geometry specification";
    throw std::invalid_argument(oss.str());
  }
  mGeometry->readFromXml(document, geomNode);

  rapidxml::xml_node<> * materialsNode = document.first_node("Materials", 0, false);
  if (materialsNode == nullptr) {
    std::ostringstream oss;
    oss << "Error in input file: Could not find materials specification";
    throw std::invalid_argument(oss.str());
  }
  mMaterialDatabase->readFromXml(document, materialsNode);

  rapidxml::xml_node<> * mesherNode = document.first_node("Mesher", 0, false);
  if (mesherNode == nullptr) {
    std::ostringstream oss;
    oss << "Error in input file: Could not find mesher specification";
    throw std::invalid_argument(oss.str());
  }
  mMesher->readFromXml(document, mesherNode);

  rapidxml::xml_node<> * solutionNode = document.first_node("Solution", 0, false);
  if (solutionNode == nullptr) {
    std::ostringstream oss;
    oss << "Error in input file: Could not find solution specification";
    throw std::invalid_argument(oss.str());
  }
  mSolution->readFromXml(document, solutionNode);
}

void InputData::validate() const
{

}
