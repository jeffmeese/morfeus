#include "application.h"

#include "commandline.h"
#include "constants.h"
#include "excitation.h"
#include "model.h"
#include "materialdatabase.h"
#include "mesh.h"
#include "meshinformation.h"
#include "mesher.h"
#include "morfeusproject.h"
#include "observation.h"
#include "solution.h"
#include "solver.h"

Application::Application(int argc, char ** argv)
  : mCommandLine(new CommandLine)
  , mProject(new morfeus::MorfeusProject)
{
  parseCommandLine(argc, argv);
}

Application::~Application()
{
}

void Application::createMesh(morfeus::mesh::Mesh * mesh)
{

}

void Application::execute()
{
  // Read the input data
  readInput();

  // Create the mesh
  std::cout << "Creating Mesh...";
  std::unique_ptr<morfeus::mesh::Mesh> mesh(new morfeus::mesh::Mesh);
  const morfeus::Mesher * mesher = mProject->mesher();
  mesh->setMaterialDatabase(mProject->materialDatabase());
  mesher->createMesh(mProject->model(), mesh.get());
  std::cout << "done\n";

  // Analyze the mesh
  std::cout << "Analyzing Mesh...";
  std::unique_ptr<morfeus::MeshInformation> meshInformation(new morfeus::MeshInformation);
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

void Application::runSolution(const morfeus::mesh::Mesh * mesh, const morfeus::MeshInformation * meshInfo)
{
  // Calculate the total number of frequencies
  morfeus::Solution * solution = mProject->solution();
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
    morfeus::Solver::vector rhs;
    for (std::size_t i = 0; i < solution->totalExcitations(); i++) {
      const morfeus::Excitation * excitation = solution->getExcitation(i);
      if (!excitation->angleDependent()) {
        excitation->excite(freqGHz, 0.0, 0.0, mesh, meshInfo, rhs);
      }
    }

    for (std::size_t j = 0; j < totalTheta; j++) {
      double theta = (thetaStart + j*thetaIncr) * morfeus::math::deg2rad;

      for (std::size_t k = 0; k < totalPhi; k++) {
        double phi = (phiStart + j*phiIncr) * morfeus::math::deg2rad;

        // Calculate angle dependent excitations
        for (std::size_t i = 0; i < solution->totalExcitations(); i++) {
          const morfeus::Excitation * excitation = solution->getExcitation(i);
          if (excitation->angleDependent()) {
            excitation->excite(freqGHz, theta, phi, mesh, meshInfo, rhs);
          }
        }

        // Solve the system
        morfeus::Solver::vector efield = solution->solver()->runSolver(freqGHz, mesh, meshInfo, rhs);

        // Solve the system and calculate the observations
        for (std::size_t i = 0; i < solution->totalObservations(); i++) {
          morfeus::observation::Observation * observation = solution->getObservation(i);
          observation->calculate(freqGHz, theta, phi, mesh, meshInfo, efield);
          reportObservation(observation);
        }
      }
    }
  }
}
