#include "solver/iterativesolution.h"

#include "excitation/planewave.h"

#include "mesh/face.h"
#include "mesh/mesh.h"
#include "mesh/meshproperties.h"

#include "math/constants.h"
#include "math/functions.h"
#include "math/matrixvectormultiply.h"
#include "math/iterativesolver.h"

#include "observation/observation.h"

#include <fstream>
#include <ctime>

IterativeSolution::IterativeSolution(const Mesh & mesh)
  : Solution(mesh)
  , mMinIterations(2)
  , mMaxIterations(5000)
  , mTolerance(1e-7)
{
  allocateMemory(mesh);
}

void IterativeSolution::allocateMemory(const Mesh &mesh)
{
  try {
    for (std::size_t i = 0; i < mesh.totalElements(); i++) {
      const Element * element = mesh.getElement(i);
      for (std::size_t j = 0; j < element->totalEdges(); j++) {
        const Edge * testEdge = mesh.getEdge(element->globalEdge(j));
        if (testEdge->unknown() > 0) {
          int testUnknown = testEdge->unknown();
          for (std::size_t k = 0; k < element->totalEdges(); k++) {
            const Edge * sourceEdge = mesh.getEdge(element->globalEdge(k));
            if (sourceEdge->unknown() > 0) {
              int sourceUnknown = sourceEdge->unknown();
              std::size_t row = static_cast<std::size_t>(testUnknown-1);
              std::size_t col = static_cast<std::size_t>(sourceUnknown-1);
              mMatrix(row, col) = math::ZERO;
            }
          }
        }
      }
    }
  }
  catch (std::bad_alloc & ex) {
    std::cout << "Out of memory" << std::endl;
    exit(1);
  }
}

void IterativeSolution::buildBiMatrix(double freq)
{
  std::cout << "  Building BI matrix..." << std::endl;

  const Mesh & mesh = this->mesh();
  double k0 = math::frequencyToWaveNumber(freq);

  // Get the list of boundary faces
  std::vector<std::unique_ptr<Face>> boundaryFaces = mesh.getBoundaryFaces();

  // Make a list of only the faces that have at least one unknown
  std::vector<Face*> usedFaces;
  for (std::size_t i = 0; i < boundaryFaces.size(); i++) {
    Face * testFace = boundaryFaces.at(i).get();
    for (std::size_t k = 0; k < testFace->totalEdges(); k++) {
      Edge * testEdge = mesh.getEdge(testFace->globalEdge(k));
      int testUnknown = testEdge->unknown();
      if (testUnknown > 0) {
        usedFaces.push_back(testFace);
        break;
      }
    }
  }


  // Build the matrix
  double currentPercent = 0.0;
  std::cout << "    0%" << std::flush;
  for (std::size_t i = 0; i < usedFaces.size(); i++) {
    Face * testFace = usedFaces.at(i);

    // Display status
    double percentComplete = double(i)*100 / usedFaces.size();
    if (percentComplete > currentPercent + 10) {
      std::cout << " " << static_cast<int>(percentComplete) << "%" << std::flush;
      currentPercent += 10;
    }

    for (std::size_t j = 0; j < usedFaces.size(); j++) {
      Face * sourceFace = usedFaces.at(j);

      dcomplex bi2 = uniformIntergal(freq, testFace, sourceFace);
      for (std::size_t k = 0; k < testFace->totalEdges(); k++) {
        Edge * testEdge = mesh.getEdge(testFace->globalEdge(k));
        int testUnknown = testEdge->unknown();
        if (testUnknown > 0) {
          double li = testEdge->computeLength(mesh);
          int si = testFace->edgeSign(k);

          for (std::size_t m = 0; m < sourceFace->totalEdges(); m++) {
            Edge * sourceEdge = mesh.getEdge(sourceFace->globalEdge(m));
            int sourceUnknown = sourceEdge->unknown();
            if (sourceUnknown > 0 && testUnknown <= sourceUnknown) {

              double lj = sourceEdge->computeLength(mesh);
              int sj = sourceFace->edgeSign(m);

              int row = testUnknown-1;
              int col = sourceUnknown-1;

              dcomplex bi1 = linearIntegral(freq, k, m, testFace, sourceFace);
              dcomplex ibi1 = -(li*lj*si*sj) / (8.0*math::PI) * bi1;
              dcomplex ibi2 =  (li*lj*si*sj) / (2.0*math::PI) * bi2;

              dcomplex val = k0*k0*ibi1 + ibi2;
              mMatrix(row, col) += val;
              if (row != col)
                mMatrix(col, row) += val;
            }
          }
        }
      }
    }
  }
  std::cout << std::endl;
}

void IterativeSolution::buildFeMatrix(double freq)
{
  std::cout << "  Building FE matrix..." << std::endl;

  const Mesh & mesh = this->mesh();

  // Calculate the matrix entries for each element
  double currentPercent = 0.0;
  std::cout << "    0%" << std::flush;
  for (std::size_t i = 0; i < mesh.totalElements(); i++) {
    const Element * element = mesh.getElement(i);
    std::size_t epsId = element->epsId();
    std::size_t muId = element->muId();

    // Display status
    double percentComplete = double(i)*100 / mesh.totalElements();
    if (percentComplete > currentPercent + 10) {
      std::cout << " " << static_cast<int>(percentComplete) << "%" << std::flush;
      currentPercent += 10;
    }

    // The input should have been validated at this point
    // so we can assume these were found
    Material * epsilon = mesh.findMaterial(epsId);
    Material * mu = mesh.findMaterial(muId);
    if (epsilon == nullptr || mu == nullptr)
      std::cerr << "Could not locate materials" << std::endl;

    for (std::size_t j = 0; j < element->totalEdges(); j++) {
      const Edge * testEdge = mesh.getEdge(element->globalEdge(j));
      int testUnknown = testEdge->unknown();
      if (testUnknown > 0) {
        for (std::size_t k = 0; k < element->totalEdges(); k++) {
          const Edge * sourceEdge = mesh.getEdge(element->globalEdge(k));
          int sourceUnknown = sourceEdge->unknown();
          if (sourceUnknown > 0) {

            // Compute the matrix entry
            dcomplex matrixEntry = element->computeMatrixEntry(epsilon, mu, mesh, freq, j, k);

            // Update the matrix entry
            int row = testUnknown - 1;
            int col = sourceUnknown - 1;
            mMatrix(row, col) += matrixEntry;
          }
        }
      }
    }
  }
  std::cout << std::endl;
}

bool IterativeSolution::buildMatrix(double freq)
{
  // Reset matrices
  mMatrix = math::ZERO;

  // Build the matrix
  buildFeMatrix(freq);
  if (meshProperties().totalApertureUnknowns() > 0)
    buildBiMatrix(freq);

  // Write out the matrix
  std::ofstream outputStream("matrix.dat");
  outputStream << mMatrix << std::endl;

  // Diagonal preconditioner
  for (std::size_t i = 0; i < meshProperties().totalUnknowns(); i++)
    mPreconditioner(i,i) = math::ONE / mMatrix(i,i);

  return true;
}

std::size_t IterativeSolution::maxIterations() const
{
  return mMaxIterations;
}

std::size_t IterativeSolution::minIterations() const
{
  return mMinIterations;
}

double IterativeSolution::tolerance() const
{
  return mTolerance;
}

void IterativeSolution::setMaxIterations(std::size_t iterations)
{
  mMaxIterations = iterations;
}

void IterativeSolution::setMinIterations(std::size_t iterations)
{
  mMinIterations = iterations;
}

void IterativeSolution::setTolerance(double tolerance)
{
  mTolerance = tolerance;
}

dcomplex_array IterativeSolution::solveSystem(const dcomplex_array & excitation)
{
  dcomplex_array efield;

  math::IterativeSolver solver;
  solver.showStatus(verbose());
  solver.setMinIterations(mMinIterations);
  solver.setMaxIterations(mMaxIterations);
  solver.setTolerance(mTolerance);

  // Create the matrix vector multiply object
  MatrixVectorMulitply mvm(this);

  // Create a random initial array
  srand((unsigned int)time(nullptr));
  dcomplex_array initialGuess(math::ZERO, excitation.size());
  double maxValue = -1.0;
  for (std::size_t i = 0; i < excitation.size(); i++){
    double re = static_cast<double>(rand()%100);
    double im = static_cast<double>(rand()%100);
    initialGuess[i] = dcomplex(re, im);
    if (abs(initialGuess[i]) > maxValue)
      maxValue = abs(initialGuess[i]);
  }
  initialGuess /= maxValue;

  efield = solver.bicg(excitation, initialGuess, mvm);
  //efield = solver.bicgstab(excitation, initialGuess, mvm);
  //efield = solver.cgs(excitation, mvm);
  std::cout << std::endl;

  return efield;
}

IterativeSolution::MatrixVectorMulitply::MatrixVectorMulitply(const IterativeSolution *solution)
  : mSolution(solution)
{
}

dcomplex_array IterativeSolution::MatrixVectorMulitply::compute(const dcomplex_array & rhs) const
{
	return mSolution->mMatrix * rhs;
}

dcomplex_array IterativeSolution::MatrixVectorMulitply::precondition(const dcomplex_array &rhs) const
{
	return mSolution->mPreconditioner * rhs;
}
