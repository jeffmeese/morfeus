#include "application.h"

#include "commandline.h"
#include "excitation.h"
#include "geometry.h"
#include "inputdata.h"
#include "material.h"
#include "mesh.h"
#include "mesher.h"
#include "observation.h"
#include "region.h"
#include "shape.h"
#include "solution.h"
#include "solver.h"

Application::Application(int argc, char ** argv)
  : mCommandLine(new CommandLine)
  , mGeometry(new Geometry)
  , mInputData(new InputData)
  , mMesh(new Mesh)
{
  parseCommandLine(argc, argv);
}

Application::~Application()
{

}

void Application::buildGeometry()
{
  // Add the shapes to the geometry
  for (std::size_t i = 0; i < mInputData->totalShapes(); i++) {
    std::unique_ptr<Shape> shape = mInputData->takeShape(i);
    shape->addToGeometry(mGeometry.get());
  }

  // Add the regions to the geometry
  for (std::size_t i = 0; i < mInputData->totalRegions(); i++) {
    std::unique_ptr<Region> region = mInputData->takeRegion(i);
    mGeometry->addRegion(std::move(region));
  }
}

void Application::createMesh()
{
  double cavityHeight = mInputData->cavityHeight();
  double freqStart = mInputData->freqStart();
  double freqStop = mInputData->freqStop();
  double maxFreq = (freqStop > freqStart) ? freqStop : freqStart;

  for (std::size_t i = 0; i < mInputData->totalMaterials(); i++) {
    std::unique_ptr<Material> material = mInputData->takeMaterial(i);
    mMesh->addMaterial(std::move(material));
  }

  // Create the surface mesh
  std::unique_ptr<Mesher> mesher(new Mesher);
  mesher->createMesh(mGeometry.get(), mMesh.get());

  // Extude the mesh to fill the cavity
}

void Application::execute()
{
  readInput();
  //buildGeometry();
  //createMesh();
  //runSolution();
}

void Application::parseCommandLine(int argc, char **argv)
{
  mFileName = argv[argc-1];
}

void Application::readInput()
{
  mInputData->readFromFile(mFileName);
  mInputData->validate();
}

void Application::runSolution()
{
  std::unique_ptr<Solution> solution = mInputData->takeSolution();
  std::unique_ptr<Solver> solver = mInputData->takeSolver();

  solution->setSolver(std::move(solver));
  for (std::size_t i = 0; i < mInputData->totalExcitations(); i++) {
    std::unique_ptr<Excitation> excitation = mInputData->takeExcitation(i);
    solution->addExcitation(std::move(excitation));
  }

  for (std::size_t i = 0; i < mInputData->totalObservations(); i++) {
    std::unique_ptr<Observation> observation = mInputData->takeObservation(i);
    solution->addObservation(std::move(observation));
  }

  double freqStart = mInputData->freqStart();
  double freqStop = mInputData->freqStop();
  double freqIncr = mInputData->freqIncrement();
  std::size_t totalFrequencies = static_cast<std::size_t>( (freqStop - freqStart) / freqIncr);
  for (std::size_t i = 0; i < totalFrequencies; i++) {
    double freq = freqStart + i*freqIncr;
    solution->runSolution(freq);
  }
}
