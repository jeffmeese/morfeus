#include "inputimpedance.h"

#include "excitation/excitation.h"
#include "excitation/probefeed.h"

#include "mesh/mesh.h"

#include "solver/solution.h"

#include <iostream>
#include <iomanip>

InputImpedance::InputImpedance(std::size_t id)
  : Observation(id, "Input Impedance")
  , mFeedId(0)
{
}

InputImpedance::InputImpedance(std::size_t id, const std::string & name)
  : Observation(id, name)
  , mFeedId(0)
{
}

InputImpedance::InputImpedance(std::size_t id, const std::string & name, std::size_t feedId)
  : Observation(id, name)
  , mFeedId(feedId)
{
}

void InputImpedance::calculate(const Solution * solution, const Mesh & mesh, double freq, const dcomplex_array & efield)
{
  Excitation * excitation = solution->findExcitation(mFeedId);

  const ProbeFeed * probeFeed = dynamic_cast<const ProbeFeed*>(excitation);
  if (probeFeed != nullptr) {
    std::size_t n1 = mesh.findNearestNode(probeFeed->x1(), probeFeed->y1(), probeFeed->z1());
    std::size_t n2 = mesh.findNearestNode(probeFeed->x2(), probeFeed->y2(), probeFeed->z2());
    Edge * edge = mesh.findEdge(n1, n2);
    int unknown = edge->unknown();
    dcomplex Ei = efield[unknown - 1];
    dcomplex zin = -(Ei / probeFeed->excitation() * edge->computeLength(mesh));

    dcomplex cap_gamma = (zin - 50.0) / (zin + 50.0);
    double mag_cap_gamma = abs(cap_gamma);
    double swr = (mag_cap_gamma != 1.0) ? (1.0 + mag_cap_gamma) / (1.0 - mag_cap_gamma) : 10.0;

    std::cout << std::setprecision(4) << std::setiosflags(std::ios::fixed) << freq
           << std::setprecision(0) << std::setiosflags(std::ios::fixed) << std::setw(8) << mFeedId
           << std::setprecision(4) << std::setiosflags(std::ios::fixed) << std::setw(24) << zin
           << std::setprecision(4) << std::setiosflags(std::ios::fixed) << std::setw(12) << swr
           << std::endl;
  }
}

std::size_t InputImpedance::feedId() const
{
  return mFeedId;
}

void InputImpedance::setFeedId(size_t feedId)
{
  mFeedId = feedId;
}
