#include "application.h"
#include "commandline.h"

#include <core/morfeusproject.h>

#include <model/model.h>

#include <model/geometry/cuboid.h>
#include <model/geometry/face.h>
#include <model/geometry/rectangle.h>
#include <model/geometry/region.h>

#include <model/media/dielectric.h>
#include <model/media/freespace.h>
#include <model/media/medialibrary.h>
#include <model/media/perfectelectricconductor.h>

#include <math/constants.h>
#include <mesh/mesh.h>
#include <mesher/mesher.h>
#include <observations/observation.h>

#include <solution/meshinformation.h>
#include <solution/solution.h>
#include <solution/solver.h>

#include <sources/source.h>

Application::Application(int argc, char ** argv)
  : mCommandLine(new CommandLine)
  , mProject(new morfeus::core::MorfeusProject)
{
  parseCommandLine(argc, argv);
}

Application::~Application()
{
}

void Application::createMesh(morfeus::mesh::Mesh * mesh)
{

}

void test()
{
  using namespace morfeus;

  std::unique_ptr<core::MorfeusProject> project(new core::MorfeusProject);
//  media::MediaLibrary * mediaLibrary = project->mediaLibrary();
  model::Model * model = project->model();

  std::unique_ptr<model::media::Dielectric> medium(new model::media::Dielectric("Dielectric"));
  medium->setAttribute(1);
  medium->setPermittivity(morfeus::math::dcomplex(2.2, 0.0));
  //mediaLibrary->addMedium(std::move(medium));

  std::unique_ptr<model::media::PerfectElectricConductor> pec(new model::media::PerfectElectricConductor);
  pec->setAttribute(2);
  //mediaLibrary->addMedium(std::move(pec));

  // Add the cavity region
  std::unique_ptr<model::geometry::Cuboid> cavity(new model::geometry::Cuboid("Cavity", model::geometry::Point(0.0, 0.0, -0.5), 2.0, 2.0, 0.5));
  model::geometry::Region * region = cavity->getRegionList().at(0);
  //region->setMedium(project->mediaLibrary()->medium(0));
  region->setLocalMeshSize(0.001);
  region->setPosition(0.0, 0.0, -0.25);

  std::vector<model::geometry::Face*> faceList = cavity->getFaceList();
//  faceList.at(1)->setMedium(mediaLibrary->medium(1));
//  faceList.at(2)->setMedium(mediaLibrary->medium(1));
//  faceList.at(3)->setMedium(mediaLibrary->medium(1));
//  faceList.at(4)->setMedium(mediaLibrary->medium(1));
//  faceList.at(5)->setMedium(mediaLibrary->medium(1));
  model->addPart(std::move(cavity));

  // Add the patch region
  std::unique_ptr<model::geometry::Rectangle> patch(new model::geometry::Rectangle("Patch", model::geometry::Point(0.0,0.0,0.0), 1.0, 1.0));
  model::geometry::Face * face = patch->getFaceList().at(0);
  //face->setMedium(mediaLibrary->medium(1));
  model->addPart(std::move(patch));

  // Create the mesh
  std::unique_ptr<mesh::Mesh> mesh(new mesh::Mesh);
  mesher::Mesher mesher;
  mesher.createMesh(model, mesh.get());

  project->print();

  std::cout << "done\n";
}

void Application::execute()
{
  test();
  return;

  // Read the input data
  readInput();

  // Create the mesh
  std::cout << "Creating Mesh...";
  std::unique_ptr<morfeus::mesh::Mesh> mesh(new morfeus::mesh::Mesh);
  const morfeus::mesher::Mesher * mesher = mProject->mesher();
  mesher->createMesh(mProject->model(), mesh.get());
  std::cout << "done\n";

  // Analyze the mesh
  std::cout << "Analyzing Mesh...";
  std::unique_ptr<morfeus::solution::MeshInformation> meshInformation(new morfeus::solution::MeshInformation);
  meshInformation->analyzeMesh(mesh.get());
  std::cout << "done\n";

  // Run the solution
  //runSolution(mesh.get(), meshInformation.get());
}

void Application::parseCommandLine(int argc, char **argv)
{
  mFileName = argv[argc-1];
}

void Application::readInput()
{
  try {
    mProject->readFromFile(mFileName);
    mProject->validate();
    mProject->print();
    mProject->saveToFile("temp.xml");
  }
  catch (std::invalid_argument & ex) {
    std::cout << ex.what() << "\n";
    exit(1);
  }
}

void Application::reportObservation(const morfeus::observation::Observation *observation)
{
  observation->report(std::cout);
}

void Application::runSolution(const morfeus::mesh::Mesh * mesh, const morfeus::solution::MeshInformation * meshInfo)
{
  // Set the media library for the solver
  morfeus::solution::Solution * solution = mProject->solution();
  //solution->solver()->setMediaLibrary(mProject->mediaLibrary());

  // Calculate the total number of frequencies
  double freqStart = solution->frequencyStart();
  double freqStop = solution->frequencyStop();
  double freqIncr = solution->frequencyIncrement();
  std::size_t totalFrequencies = static_cast<std::size_t>( (freqStop-freqStart) / freqIncr);
  if (totalFrequencies <= 0)
    totalFrequencies = 1;

  // Calculate the total number of theta angles
  double thetaStart = solution->thetaStart();
  double thetaStop = solution->thetaStop();
  double thetaIncr = solution->thetaIncrement();
  std::size_t totalTheta = static_cast<std::size_t>( (thetaStop-thetaStart) / thetaIncr);
  if (totalTheta <= 0)
    totalTheta = 1;

  // Calculate the total number of phi angles
  double phiStart = solution->phiStart();
  double phiStop = solution->phiStop();
  double phiIncr = solution->phiIncrement();
  std::size_t totalPhi = static_cast<std::size_t>( (phiStop-phiStart) / phiIncr);
  if (totalPhi <= 0)
    totalPhi = 1;

  // Run the solver at each frequency and angle specified
  for (std::size_t i = 0; i < totalFrequencies; i++) {
    double freqGHz = freqStart + i*freqIncr;

    // Calculate non angle dependent excitations
    morfeus::math::vector rhs;
    for (std::size_t i = 0; i < solution->totalSources(); i++) {
      const morfeus::sources::Source * source = solution->source(i);
      if (!source->angleDependent()) {
        source->excite(freqGHz, 0.0, 0.0, mesh, meshInfo, rhs);
      }
    }

    for (std::size_t j = 0; j < totalTheta; j++) {
      double theta = (thetaStart + j*thetaIncr) * morfeus::math::deg2rad;

      for (std::size_t k = 0; k < totalPhi; k++) {
        double phi = (phiStart + j*phiIncr) * morfeus::math::deg2rad;

        // Calculate angle dependent excitations
        for (std::size_t i = 0; i < solution->totalSources(); i++) {
          const morfeus::sources::Source * source = solution->source(i);
          if (source->angleDependent()) {
            source->excite(freqGHz, theta, phi, mesh, meshInfo, rhs);
          }
        }

        // Solve the system
        morfeus::math::vector efield = solution->solver()->runSolver(freqGHz, mesh, meshInfo, rhs);

        // Solve the system and calculate the observations
        for (std::size_t i = 0; i < solution->totalObservations(); i++) {
          morfeus::observation::Observation * observation = solution->observation(i);
          observation->calculate(freqGHz, theta, phi, mesh, meshInfo, efield);
          reportObservation(observation);
        }
      }
    }
  }
}
