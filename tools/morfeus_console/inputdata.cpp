#include "inputdata.h"

#include "excitation.h"
#include "geometry.h"
#include "hole.h"
#include "material.h"
#include "observation.h"
#include "region.h"
#include "segment.h"
#include "shape.h"
#include "solution.h"
#include "solver.h"
#include "vertex.h"

#include <boost/property_tree/xml_parser.hpp>

InputData::InputData()
  : mCavityHeight(0.0)
  , mFreqIncrement(0.0)
  , mFreqStart(0.0)
  , mFreqStop(0.0)
{
}

InputData::~InputData()
{

}

void InputData::addExcitation(std::unique_ptr<Excitation> excitation)
{
  mExcitations.push_back(std::move(excitation));
}

void InputData::addMaterial(std::unique_ptr<Material> material)
{
  mMaterials.push_back(std::move(material));
}

void InputData::addObservation(std::unique_ptr<Observation> observation)
{
  mObservations.push_back(std::move(observation));
}

void InputData::addRegion(std::unique_ptr<Region> region)
{
  mRegions.push_back(std::move(region));
}

void InputData::addShape(std::unique_ptr<Shape> shape)
{
  return mShapes.push_back(std::move(shape));
}

void InputData::readFromFile(const std::string & fileName)
{
  std::cout << "Read\n";

  using namespace boost::property_tree;

  std::ifstream input(fileName);
  if (input.fail()) {
    std::cout << "File error\n";
    return;
  }
  ptree rootTree;
  read_xml(fileName, rootTree);

  for (auto itr = rootTree.begin(); itr != rootTree.end(); ++itr) {
    std::cout << itr->first << "\n";
    ptree child = itr->second.get_child("geometry");
    std::cout << child.begin()->first << "\n";
  }

  //ptree project = tree.get_child("project");
  //ptree geometry = project.get_child("geometry");
  //Geometry geometry = tree.get<Geometry>("project.geometry");
}

std::unique_ptr<Excitation> InputData::takeExcitation(std::size_t index)
{
  std::unique_ptr<Excitation> excitation(mExcitations.at(index).release());
  mExcitations.erase(mExcitations.begin() + index);
  return excitation;
}

std::unique_ptr<Material> InputData::takeMaterial(std::size_t index)
{
  std::unique_ptr<Material> material(mMaterials.at(index).release());
  mMaterials.erase(mMaterials.begin() + index);
  return material;
}

std::unique_ptr<Observation> InputData::takeObservation(std::size_t index)
{
  std::unique_ptr<Observation> observation(mObservations.at(index).release());
  mObservations.erase(mObservations.begin() + index);
  return observation;
}

std::unique_ptr<Region> InputData::takeRegion(std::size_t index)
{
  std::unique_ptr<Region> region(mRegions.at(index).release());
  mRegions.erase(mRegions.begin() + index);
  return region;
}

std::unique_ptr<Shape> InputData::takeShape(std::size_t index)
{
  std::unique_ptr<Shape> shape(mShapes.at(index).release());
  mShapes.erase(mShapes.begin() + index);
  return shape;
}

std::unique_ptr<Solution> InputData::takeSolution()
{
  std::unique_ptr<Solution> solution(mSolution.release());
  return solution;
}

std::unique_ptr<Solver> InputData::takeSolver()
{
  std::unique_ptr<Solver> solver(mSolver.release());
  return solver;
}

void InputData::validate() const
{

}
