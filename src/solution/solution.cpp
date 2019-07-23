#include "solution.h"

#include "iterativesolver.h"
#include "excitation.h"
#include "materialdatabase.h"
#include "observation.h"
#include "solver.h"

Solution::Solution()
  : MorfeusObject("Solution")
  , mSolver(new IterativeSolver)
{
  mFreqIncr = mFreqStop = mFreqStart = 0.0;
}

void Solution::addExcitation(std::unique_ptr<Excitation> excitation)
{
  mExcitations.push_back(std::move(excitation));
}

void Solution::addObservation(std::unique_ptr<Observation> observation)
{
  mObservations.push_back(std::move(observation));
}

void Solution::print(std::ostream &output, int tabPos) const
{
  xmlutils::printHeader(output, tabPos, "Solution");

  xmlutils::printHeader(output, tabPos+2, "Solver");
  mSolver->print(output, tabPos+4);

  xmlutils::printHeader(output, tabPos+2, "Excitations");
  for (std::size_t i = 0; i < mExcitations.size(); i++) {
    mExcitations.at(i)->print(output, tabPos+4);
  }

  xmlutils::printHeader(output, tabPos+2, "Observations");
  for (std::size_t i = 0; i < mObservations.size(); i++) {
    mObservations.at(i)->print(output, tabPos+4);
  }
  xmlutils::printHeader(output, tabPos, "End Solution");
  output << "\n";
}

void Solution::print(int tabPos) const
{
  print(std::cout, tabPos);
}

void Solution::readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  setFrequencyStart(xmlutils::readAttribute<double>(node, "freq-start"));
  setFrequencyStop(xmlutils::readAttribute<double>(node, "freq-stop"));
  setFrequencyIncrement(xmlutils::readAttribute<double>(node, "freq-incr"));

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
      std::unique_ptr<Excitation> excitation(Excitation::factory().create(type));
      if (excitation != nullptr) {
        excitation->readFromXml(document, excitationNode);
        addExcitation(std::move(excitation));
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
      std::unique_ptr<Observation> observation(Observation::factory().create(type));
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

void Solution::writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * solutionNode = xmlutils::createNode(document, "Solution");

  xmlutils::writeAttribute(document, solutionNode, "freq-start", mFreqStart);
  xmlutils::writeAttribute(document, solutionNode, "freq-stop", mFreqStop);
  xmlutils::writeAttribute(document, solutionNode, "freq-incr", mFreqIncr);

  mSolver->writeToXml(document, solutionNode);

  rapidxml::xml_node<> * excitationsNode = xmlutils::createNode(document, "Excitations");
  for (std::size_t i = 0; i < mExcitations.size(); i++) {
    const Excitation * excitation = mExcitations.at(i).get();
    excitation->writeToXml(document, excitationsNode);
  }
  solutionNode->append_node(excitationsNode);

  rapidxml::xml_node<> * observationsNode = xmlutils::createNode(document, "Observations");
  for (std::size_t i = 0; i < mObservations.size(); i++) {
    const Observation * observation = mObservations.at(i).get();
    observation->writeToXml(document, observationsNode);
  }
  solutionNode->append_node(observationsNode);

  node->append_node(solutionNode);
}

void Solution::runSolution(const Mesh * mesh)
{
  std::size_t totalFrequencies = static_cast<std::size_t>( (mFreqStop - mFreqStart) / mFreqIncr);
  for (std::size_t i = 0; i < totalFrequencies; i++) {
    double freqGHz = mFreqStart + i*mFreqIncr;
    mSolver->runSolver(freqGHz, mesh, this);
  }
}

void Solution::setSolver(std::unique_ptr<Solver> solver)
{
  mSolver = std::move(solver);
}
