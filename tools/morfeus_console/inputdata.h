#ifndef INPUTDATA_H
#define INPUTDATA_H

#include <memory>
#include <string>
#include <vector>

class Geometry;
class MaterialDatabase;
class Mesher;
class Solution;

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

public:
  const Geometry * geometry() const;
  const MaterialDatabase * materialDatabase() const;
  const Mesher * mesher() const;
  const Solution * solution() const;
  Solution * solution();

public:
  void readFromFile(const std::string & fileName);
  void validate() const;

private:
  double mCavityHeight;
  double mFreqIncrement;
  double mFreqStart;
  double mFreqStop;
  std::unique_ptr<Geometry> mGeometry;
  std::unique_ptr<MaterialDatabase> mMaterialDatabase;
  std::unique_ptr<Mesher> mMesher;
  std::unique_ptr<Solution> mSolution;
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

inline const Geometry * InputData::geometry() const
{
  return mGeometry.get();
}

inline const MaterialDatabase * InputData::materialDatabase() const
{
  return mMaterialDatabase.get();
}

inline const Mesher * InputData::mesher() const
{
  return mMesher.get();
}

inline const Solution * InputData::solution() const
{
  return mSolution.get();
}

inline Solution * InputData::solution()
{
  return mSolution.get();
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

#endif // INPUTDATA_H
