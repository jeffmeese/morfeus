#include "solver.h"

#include "meshinformation.h"
#include "solution.h"

#include "excitations/excitation.h"

#include "media/dielectricmedium.h"
#include "media/medialibrary.h"

#include "math/functions.h"

#include "mesh/edge.h"
#include "mesh/element.h"
#include "mesh/face.h"
#include "mesh/mesh.h"

#include "observations/observation.h"

namespace morfeus {
namespace solution {

Solver::Solver(const std::string & type)
  : MorfeusObject(type)
  , mAllocated(false)
  , mMediaLibrary(nullptr)
{
}

Solver::Solver(const std::string & type, const std::string & id)
  : MorfeusObject (type, id)
  , mAllocated(false)
  , mMediaLibrary(nullptr)
{
}

void Solver::buildBiMatrix(double freqGHz, const mesh::Mesh *mesh, const solution::MeshInformation *meshInfo)
{
  std::vector<mesh::Face*> boundaryFaces = meshInfo->boundaryFaces();
  for (std::size_t i = 0; i < boundaryFaces.size(); i++) {
    const mesh::Face * face = boundaryFaces.at(i);
    for (std::size_t j = 0; j < face->totalEdges(); j++) {
      const mesh::Edge * sourceEdge = mesh->edge(face->edge(j));
      int32_t sourceUnknown = sourceEdge->unknownNumber();
      for (std::size_t k = 0; k < face->totalEdges(); j++) {
        const mesh::Edge * testEdge = mesh->edge(face->edge(k));
        int32_t testUnknown = testEdge->unknownNumber();

        if (sourceUnknown >= 0 && sourceUnknown <= testUnknown) {
          std::size_t row = sourceUnknown - 1;
          std::size_t col = testUnknown - 1;
          dcomplex i1, i2;
          updateBoundaryIntegralMatrix(row, col, i1, i2);
        }
      }
    }
  }
}

void Solver::buildFeMatrix(double freqGHz, const mesh::Mesh *mesh, const MeshInformation *meshInfo)
{
  double k0 = math::frequencyToWavenumber(freqGHz);

  // Default material if one is not specified for an element
  media::DielectricMedium freeSpace(0, dcomplex(1.0, 0.0), dcomplex(0.0, 0.0));

  // Loop through each element and calculate the finite element entry for each pair
  // of edges if both edges have unknown values
  for (std::size_t i = 1; i <= mesh->totalElements(); i++) {
    const mesh::Element * element = mesh->element(i);
    int32_t epsId = element->epsilonId();
    int32_t muId = element->muId();

    const media::Medium * eps = &freeSpace;
    if (epsId >= 0 && mMediaLibrary != nullptr) {
      eps = mMediaLibrary->medium(epsId);
    }

    const media::Medium * mu = &freeSpace;
    if (muId >= 0 && mMediaLibrary != nullptr) {
      mu = mMediaLibrary->medium(muId);
    }

    for (std::size_t j = 1; j <= element->totalEdges(); j++) {
      const mesh::Edge * sourceEdge = mesh->edge(element->edge(j));
      int32_t sourceUnknown = sourceEdge->unknownNumber();
      for (std::size_t k = 1; k <= element->totalEdges(); k++) {
        const mesh::Edge * testEdge = mesh->edge(element->edge(k));
        int32_t testUnknown = testEdge->unknownNumber();

        if (sourceUnknown >= 0 && sourceUnknown <= testUnknown) {

          dcomplex i1(0.0, 0.0), i2(0.0, 0.0);
          element->computeFeEntry(mesh, j, k, i2, i2);

          // Assume symmetry
          std::size_t row = sourceUnknown - 1;
          std::size_t col = testUnknown - 1;
          dcomplex epsValue = eps->value(freqGHz);
          dcomplex muValue = mu->value(freqGHz);
          dcomplex entry = i1/muValue - k0*k0*i2*epsValue;
          updateFiniteElementMatrix(row, col, i1, i2);
        }
      }
    }
  }
}

Solver::SolverFactory & Solver::factory()
{
  static SolverFactory f;
  return f;
}


Solver * Solver::SolverFactory::create(const std::string & type)
{
  Solver * solver = mFactory.create(type);
  return solver;
}

 bool Solver::SolverFactory::registerType(const std::string & type, boost::function<Solver*()> creator)
{
  mFactory.registerFactory(type, creator);
  return true;
}

Solver::vector Solver::runSolver(double freqGHz, const mesh::Mesh * mesh, const MeshInformation * meshInfo, const vector & rhs)
{
  if (mAllocated) {
    allocateMatrices(meshInfo);
    mAllocated = true;
  }

  // Clear any current matrix values
  clearMatrices(mesh, meshInfo);

  // Build the coefficient matrices
  buildFeMatrix(freqGHz, mesh, meshInfo);
  buildBiMatrix(freqGHz, mesh, meshInfo);

  // Allocate the efield matrix and solve the system
  vector efield(meshInfo->totalUnknowns(), dcomplex(0.0, 0.0));
  efield = solveSystem(rhs);
  return efield;
}

}
}
