#ifndef SOLUTION_H
#define SOLUTION_H

#include "morfeus.h"
#include "types.h"

#include "math/vector.h"

#include "mesh/meshproperties.h"

#include <memory>

class InputData;
class Mesh;
class Face;
class Excitation;
class Observation;
class Planewave;

class Solution
{
public:
  MORFEUS_LIB_DECL const Mesh & mesh() const;
  MORFEUS_LIB_DECL const MeshProperties & meshProperties() const;
  MORFEUS_LIB_DECL bool verbose() const;
  MORFEUS_LIB_DECL std::size_t totalFrequencies() const;
  MORFEUS_LIB_DECL std::size_t totalNonPlanewaveExcitations() const;
  MORFEUS_LIB_DECL std::size_t totalObservations() const;
  MORFEUS_LIB_DECL std::size_t totalPlanewaves() const;
  MORFEUS_LIB_DECL void setVerbose(bool verbose);

public:
  MORFEUS_LIB_DECL void addFrequency(double freqGHz);
  MORFEUS_LIB_DECL void addNonPlanewaveExcitation(std::unique_ptr<Excitation> excitation);
  MORFEUS_LIB_DECL void addObservation(std::unique_ptr<Observation> observation);
  MORFEUS_LIB_DECL void addPlanewave(std::unique_ptr<Planewave> planewave);
  MORFEUS_LIB_DECL Excitation * findExcitation(std::size_t id) const;
  MORFEUS_LIB_DECL double frequency(std::size_t index) const;
  MORFEUS_LIB_DECL Excitation * nonPlanewaveExcitation(std::size_t index) const;
  MORFEUS_LIB_DECL Observation * observation(std::size_t index) const;
  MORFEUS_LIB_DECL Planewave * planewave(std::size_t index) const;
  MORFEUS_LIB_DECL virtual bool postProcess(double freqGHz, const dcomplex_array & efield);
  MORFEUS_LIB_DECL void writeExcitationArray(const dcomplex_array & array, std::ostream & outputStream);
  MORFEUS_LIB_DECL void writeFieldArray(const dcomplex_array & efield, std::ostream & outputStream);

public:
  MORFEUS_LIB_DECL virtual bool buildMatrix(double freqGHz) = 0;
  MORFEUS_LIB_DECL virtual dcomplex_array solveSystem(const dcomplex_array & excitation) = 0;

protected:
  Solution(const Mesh & mesh);
  dcomplex linearIntegral(double freq, size_t test, size_t source, Face * testFace, Face * sourceFace) const;
  math::vector<double> singularLinearIntegral(double x, double y, double * xi, double * yi) const;
  double singularUniformIntegral(double x, double y, double * xi, double * yi) const;
  dcomplex uniformIntergal(double freq, Face * testFace, Face * sourceFace) const;

private:
  const Mesh & mMesh;
  MeshProperties mMeshProperties;
  bool mVerbose;
  std::vector<double> mFrequencies;
  std::vector<std::unique_ptr<Excitation>> mNonPlanewaveExcitations;
  std::vector<std::unique_ptr<Observation>> mObservations;
  std::vector<std::unique_ptr<Planewave>> mPlanewaves;
};

#endif // SOLUTION_H
