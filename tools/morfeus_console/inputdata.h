#ifndef INPUTDATA_H
#define INPUTDATA_H

#include <string>
#include <vector>

class Excitation;
class Material;
class Observation;
class Region;
class Shape;
class Solution;
class Solver;

class InputData
{
public:
  InputData();
  ~InputData();

public:
  double cavityHeight() const;
  double freqIncrement() const;
  double freqStart() const;
  double freqStop() const;
  void setCavityHeight(double value);
  void setFreqIncrement(double value);
  void setFreqStart(double value);
  void setFreqStop(double value);
  std::size_t totalExcitations() const;
  std::size_t totalMaterials() const;
  std::size_t totalObservations() const;
  std::size_t totalRegions() const;
  std::size_t totalShapes() const;

public:
  void addExcitation(std::unique_ptr<Excitation> excitation);
  void addMaterial(std::unique_ptr<Material> material);
  void addObservation(std::unique_ptr<Observation> observation);
  void addRegion(std::unique_ptr<Region> region);
  void addShape(std::unique_ptr<Shape> shape);
  void readFromFile(const std::string & fileName);
  void setSolution(std::unique_ptr<Solution> solution);
  void setSolver(std::unique_ptr<Solver> solver);
  std::unique_ptr<Excitation> takeExcitation(std::size_t index);
  std::unique_ptr<Material> takeMaterial(std::size_t index);
  std::unique_ptr<Observation> takeObservation(std::size_t index);
  std::unique_ptr<Shape> takeShape(std::size_t index);
  std::unique_ptr<Region> takeRegion(std::size_t index);
  std::unique_ptr<Solution> takeSolution();
  std::unique_ptr<Solver> takeSolver();
  void validate() const;

private:
  typedef std::unique_ptr<Excitation> ExcitationPtr;
  typedef std::unique_ptr<Material> MaterialPtr;
  typedef std::unique_ptr<Observation> ObservationPtr;
  typedef std::unique_ptr<Region> RegionPtr;
  typedef std::unique_ptr<Shape> ShapePtr;
  typedef std::vector<ExcitationPtr> ExcitationVector;
  typedef std::vector<MaterialPtr> MaterialVector;
  typedef std::vector<ObservationPtr> ObservationVector;
  typedef std::vector<RegionPtr> RegionVector;
  typedef std::vector<ShapePtr> ShapeVector;
  typedef std::unique_ptr<Solution> SolutionPtr;
  typedef std::unique_ptr<Solver> SolverPtr;

private:
  double mCavityHeight;
  double mFreqIncrement;
  double mFreqStart;
  double mFreqStop;
  ExcitationVector mExcitations;
  MaterialVector mMaterials;
  ObservationVector mObservations;
  RegionVector mRegions;
  ShapeVector mShapes;
  SolutionPtr mSolution;
  SolverPtr mSolver;
};

inline double InputData::cavityHeight() const
{
  return mCavityHeight;
}

inline double InputData::freqIncrement() const
{
  return mFreqIncrement;
}

inline double InputData::freqStart() const
{
  return mFreqStart;
}

inline double InputData::freqStop() const
{
  return mFreqStop;
}

inline void InputData::setCavityHeight(double value)
{
  mCavityHeight = value;
}

inline void InputData::setFreqIncrement(double value)
{
  mFreqIncrement = value;
}

inline void InputData::setFreqStart(double value)
{
  mFreqStart = value;
}

inline void InputData::setFreqStop(double value)
{
  mFreqStop = value;
}

inline std::size_t InputData::totalExcitations() const
{
  return mExcitations.size();
}

inline std::size_t InputData::totalMaterials() const
{
  return mMaterials.size();
}

inline std::size_t InputData::totalObservations() const
{
  return mObservations.size();
}

inline std::size_t InputData::totalRegions() const
{
  return mRegions.size();
}

inline std::size_t InputData::totalShapes() const
{
  return mShapes.size();
}

#endif // INPUTDATA_H
