#include "solution.h"

#include "solver.h"
#include "iterativesolver.h"

#include "sources/source.h"

#include "model/media/medialibrary.h"

#include "observations/observation.h"

namespace morfeus {
namespace solution {

Solution::Solution()
  : MorfeusObject("Solution")
  , mSolver(new IterativeSolver)
{
  mFreqIncr = mFreqStop = mFreqStart = 0.0;
  mThetaIncr = mThetaStop = mThetaStart = 0.0;
  mPhiIncr = mPhiStop = mPhiStart = 0.0;
}

void Solution::addSource(std::unique_ptr<sources::Source> source)
{
  mSources.push_back(std::move(source));
}

void Solution::addObservation(std::unique_ptr<observation::Observation> observation)
{
  mObservations.push_back(std::move(observation));
}

void Solution::doPrint(std::ostream &output, int tabPos) const
{
  xmlutils::printValue(output, tabPos, "Freq Start: ", mFreqStart);
  xmlutils::printValue(output, tabPos, "Freq Stop: ", mFreqStop);
  xmlutils::printValue(output, tabPos, "Freq Incr: ", mFreqIncr);
  xmlutils::printValue(output, tabPos, "Theta Start: ", mThetaStart);
  xmlutils::printValue(output, tabPos, "Theta Stop: ", mThetaStop);
  xmlutils::printValue(output, tabPos, "Theta Incr: ", mThetaIncr);
  xmlutils::printValue(output, tabPos, "Phi Start: ", mPhiStart);
  xmlutils::printValue(output, tabPos, "Phi Stop: ", mPhiStop);
  xmlutils::printValue(output, tabPos, "Phi Incr: ", mPhiIncr);

  xmlutils::printHeader(output, tabPos+2, "Solver");
  mSolver->print(output, tabPos+4);
  xmlutils::printHeader(output, tabPos+2, "End Solver");

  xmlutils::printHeader(output, tabPos+2, "Sources");
  for (std::size_t i = 0; i < mSources.size(); i++) {
    mSources.at(i)->print(output, tabPos+4);
  }
  xmlutils::printHeader(output, tabPos+2, "End Excitations");

  xmlutils::printHeader(output, tabPos+2, "Observations");
  for (std::size_t i = 0; i < mObservations.size(); i++) {
    mObservations.at(i)->print(output, tabPos+4);
  }
  xmlutils::printHeader(output, tabPos+2, "End Observations");

  output << "\n";
}

void Solution::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setFrequencyStart(xmlutils::readAttribute<double>(node, "freq-start"));
  setFrequencyStop(xmlutils::readAttribute<double>(node, "freq-stop"));
  setFrequencyIncrement(xmlutils::readAttribute<double>(node, "freq-incr"));

  setThetaStart(xmlutils::readAttribute<double>(node, "theta-start"));
  setThetaStop(xmlutils::readAttribute<double>(node, "theta-stop"));
  setThetaIncrement(xmlutils::readAttribute<double>(node, "theta-incr"));

  setPhiStart(xmlutils::readAttribute<double>(node, "phi-start"));
  setPhiStop(xmlutils::readAttribute<double>(node, "phi-stop"));
  setPhiIncrement(xmlutils::readAttribute<double>(node, "phi-incr"));

  rapidxml::xml_node<> * solverNode = node->first_node("Solver", 0, false);
  if (solverNode == nullptr) {
    std::cerr << "Warning: Solver section not found in solution specification\n";
  }

  if (solverNode != nullptr) {
    std::string type = xmlutils::readAttribute<std::string>(solverNode, "type");
    std::unique_ptr<Solver> solver(Solver::factory().create(type));
    if (solver != nullptr) {
      solver->readFromXml(document, solverNode);
      setSolver(std::move(solver));
    }
    else {
      std::ostringstream oss;
      oss << "Warning: Solver type " << type << " not recognized";
      std::cerr << oss.str() << "\n";
    }
  }

  rapidxml::xml_node<> * excitationsNode = node->first_node("Excitations", 0, false);
  if (excitationsNode == nullptr) {
    std::cerr << "Warning: Excitations section not found in solution specification\n";
  }

  if (excitationsNode != nullptr) {
    rapidxml::xml_node<> * excitationNode = excitationsNode->first_node("Excitation", 0, false);
    while (excitationNode != nullptr) {
      std::string type = xmlutils::readAttribute<std::string>(excitationNode, "type");
      std::unique_ptr<sources::Source> source(sources::Source::factory().create(type));
      if (source != nullptr) {
        source->readFromXml(document, excitationNode);
        addSource(std::move(source));
      }
      else {
        std::ostringstream oss;
        oss << "Warning: Excitation type " << type << " not recognized";
        std::cerr << oss.str() << "\n";
      }

      // Shape factory needed here
      excitationNode = excitationNode->next_sibling("Excitation", 0, false);
    }
  }

  rapidxml::xml_node<> * observationsNode = node->first_node("Observations", 0, false);
  if (observationsNode == nullptr) {
    std::cerr << "Warning: Observation section not found in solution specification\n";
  }

  if (observationsNode != nullptr) {
    rapidxml::xml_node<> * observationNode = observationsNode->first_node("Observation", 0, false);
    while (observationNode != nullptr) {
      std::string type = xmlutils::readAttribute<std::string>(observationNode, "type");
      std::unique_ptr<observation::Observation> observation(observation::Observation::factory().create(type));
      if (observation != nullptr) {
        observation->readFromXml(document, observationNode);
        addObservation(std::move(observation));
      }
      else {
        std::ostringstream oss;
        oss << "Warning: Observation type " << type << " not recognized";
        std::cerr << oss.str() << "\n";
      }

      // Shape factory needed here
      observationNode = observationNode->next_sibling("Observation", 0, false);
    }
  }
}

void Solution::setSolver(std::unique_ptr<Solver> solver)
{
  mSolver = std::move(solver);
}

void Solution::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  xmlutils::writeAttribute(document, node, "freq-start", mFreqStart);
  xmlutils::writeAttribute(document, node, "freq-stop", mFreqStop);
  xmlutils::writeAttribute(document, node, "freq-incr", mFreqIncr);
  xmlutils::writeAttribute(document, node, "theta-start", mThetaStart);
  xmlutils::writeAttribute(document, node, "theta-stop", mThetaStop);
  xmlutils::writeAttribute(document, node, "theta-incr", mThetaIncr);
  xmlutils::writeAttribute(document, node, "phi-start", mPhiStart);
  xmlutils::writeAttribute(document, node, "phi-stop", mPhiStop);
  xmlutils::writeAttribute(document, node, "phi-incr", mPhiIncr);

  rapidxml::xml_node<> * solverNode = xmlutils::createNode(document, "Solver");
  mSolver->writeToXml(document, solverNode);
  node->append_node(solverNode);

  rapidxml::xml_node<> * excitationsNode = xmlutils::createNode(document, "Excitations");
  for (std::size_t i = 0; i < mSources.size(); i++) {
    const sources::Source * source = mSources.at(i).get();
    source->writeToXml(document, excitationsNode);
  }
  node->append_node(excitationsNode);

  rapidxml::xml_node<> * observationsNode = xmlutils::createNode(document, "Observations");
  for (std::size_t i = 0; i < mObservations.size(); i++) {
    const observation::Observation * observation = mObservations.at(i).get();
    observation->writeToXml(document, observationsNode);
  }
  node->append_node(observationsNode);
}

}
}
