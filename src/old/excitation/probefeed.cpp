#include "excitation/probefeed.h"

#include "mesh/mesh.h"

#include "math/constants.h"
#include "math/functions.h"

ProbeFeed::ProbeFeed(std::size_t id)
  : Excitation(id)
{
  mX1 = 0.0;
  mY1 = 0.0;
  mZ1 = 0.0;
  mX2 = 0.0;
  mY2 = 0.0;
  mZ2 = 0.0;
  mExcitation = dcomplex(1.0,0.0);
}

ProbeFeed::ProbeFeed(std::size_t id, double x1, double y1, double z1, double x2, double y2, double z2, const dcomplex & excitation)
  : Excitation(id)
{
  mX1 = x1;
  mY1 = y1;
  mZ1 = z1;
  mX2 = x2;
  mY2 = y2;
  mZ2 = z2;
  mExcitation = excitation;
}

void ProbeFeed::excite(const Mesh &mesh, double freq, dcomplex_array & rhs) const
{
  double k0 = math::frequencyToWaveNumber(freq);

  //double lambda = 1.0 / freq;
  //double k0 = 2.0*math::PI/lambda;

  //double lam_cm = 30.0 / freqGHz;
  //double k0 = 2.0 * math::PI / lam_cm;

  size_t n1 = mesh.findNearestNode(mX1, mY1, mZ1);
  size_t n2 = mesh.findNearestNode(mX2, mY2, mZ2);
  Edge * edge = mesh.findEdge(n1, n2);
  int unknown = edge->unknown();

  rhs[unknown-1] = -(math::CJ * k0 * math::Z0 * mExcitation * edge->computeLength(mesh));
}

dcomplex ProbeFeed::excitation() const
{
  return mExcitation;
}

void ProbeFeed::setX1(double x1)
{
  mX1 = x1;
}

void ProbeFeed::setX2(double x2)
{
  mX2 = x2;
}

void ProbeFeed::setY1(double y1)
{
  mY1 = y1;
}

void ProbeFeed::setY2(double y2)
{
  mY2 = y2;
}

void ProbeFeed::setZ1(double z1)
{
  mZ1 = z1;
}

void ProbeFeed::setZ2(double z2)
{
  mZ2 = z2;
}

void ProbeFeed::setPosition(double x1, double y1, double z1, double x2, double y2, double z2)
{
  mX1 = x1;
  mY1 = y1;
  mZ1 = z1;
  mX2 = x2;
  mY2 = y2;
  mZ2 = z2;
}

void ProbeFeed::setExcitation(const dcomplex & value)
{
  mExcitation = value;
}

double ProbeFeed::x1() const
{
  return mX1;
}

double ProbeFeed::x2() const
{
  return mX2;
}

double ProbeFeed::y1() const
{
  return mY1;
}

double ProbeFeed::y2() const
{
  return mY2;
}

double ProbeFeed::z1() const
{
  return mZ1;
}

double ProbeFeed::z2() const
{
  return mZ2;
}
