#ifndef INPUTDATA_H
#define INPUTDATA_H

#include <string>

class InputData
{
public:
  InputData();

public:
  double freqIncrement() const;
  double freqStart() const;
  double freqStop() const;
  std::string geomFile() const;
  double layerThickness(std::size_t index) const;
  std::size_t numLayers() const;
  double phiIncrement() const;
  double phiStart() const;
  double phiStop() const;
  double thetaIncrement() const;
  double thetaStart() const;
  double thetaStop() const;

public:
  void readFromFile(const std::string & fileName);

private:
  std::string mGeomFile;
  double * mLayerThickness;
};

#endif // INPUTDATA_H
