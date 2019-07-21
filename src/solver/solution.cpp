#include "solution.h"

#include "iterativesolver.h"
#include "excitation.h"
#include "materialdatabase.h"
#include "observation.h"
#include "solver.h"

Solution::Solution()
  : mSolver(new IterativeSolver)
{
}

Solution::~Solution()
{
}

void Solution::addExcitation(std::unique_ptr<Excitation> excitation)
{
  mExcitations.push_back(std::move(excitation));
}

void Solution::addObservation(std::unique_ptr<Observation> observation)
{
  mObservations.push_back(std::move(observation));
}

void Solution::doPrint(std::ostream &output, int tabPos) const
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
}

void Solution::doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node)
{
  rapidxml::xml_node<> * solverNode = node->first_node("Solver", 0, false);
  if (solverNode != nullptr) {
    std::string type = xmlutils::readAttribute<std::string>(solverNode, "type");
    try {
      std::unique_ptr<Solver> solver(Solver::Factory::Instance().CreateObject(type));
      solver->readFromXml(document, solverNode);
      setSolver(std::move(solver));
    }
    catch (Loki::DefaultFactoryError<std::string, Solver>::Exception &) {
      std::ostringstream oss;
      oss << "Could not create solver with type " << type;
      return;
    }
  }

  rapidxml::xml_node<> * excitationsNode = node->first_node("Excitations", 0, false);
  if (excitationsNode != nullptr) {
    rapidxml::xml_node<> * excitationNode = node->first_node("Excitation", 0, false);
    while (excitationNode != nullptr) {
      std::string type = xmlutils::readAttribute<std::string>(excitationNode, "type");

      try {
        std::unique_ptr<Excitation> excitation(Excitation::Factory::Instance().CreateObject(type));
        excitation->readFromXml(document, excitationNode);
        addExcitation(std::move(excitation));
      }
      catch (Loki::DefaultFactoryError<std::string, Excitation>::Exception &) {
        std::ostringstream oss;
        oss << "Could not create shape with type " << type;
        return;
      }

      // Shape factory needed here
      excitationNode = excitationNode->next_sibling("Excitation", 0, false);
    }
  }

  rapidxml::xml_node<> * observationsNode = node->first_node("Observations", 0, false);
  if (observationsNode != nullptr) {
    rapidxml::xml_node<> * observationNode = node->first_node("Observation", 0, false);
    while (observationNode != nullptr) {
      std::string type = xmlutils::readAttribute<std::string>(observationNode, "type");

      try {
        std::unique_ptr<Observation> observation(Observation::Factory::Instance().CreateObject(type));
        observation->readFromXml(document, observationNode);
        addObservation(std::move(observation));
      }
      catch (Loki::DefaultFactoryError<std::string, Observation>::Exception &) {
        std::ostringstream oss;
        oss << "Could not create shape with type " << type;
        return;
      }

      // Shape factory needed here
      observationNode = observationNode->next_sibling("Observation", 0, false);
    }
  }
}

void Solution::doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const
{
  rapidxml::xml_node<> * solutionNode = xmlutils::createNode(document, "Solution");

  rapidxml::xml_node<> * solverNode = xmlutils::createNode(document, "Solver");
  mSolver->writeToXml(document, solverNode);

  rapidxml::xml_node<> * excitationsNode = xmlutils::createNode(document, "Excitations");
  for (std::size_t i = 0; i < mExcitations.size(); i++) {
    rapidxml::xml_node<> * childNode = xmlutils::createNode(document, "Excitation");
    const Excitation * excitation = mExcitations.at(i).get();
    excitation->writeToXml(document, childNode);
    excitationsNode->append_node(childNode);
  }
  solutionNode->append_node(excitationsNode);

  rapidxml::xml_node<> * observationsNode = xmlutils::createNode(document, "Observations");
  for (std::size_t i = 0; i < mObservations.size(); i++) {
    rapidxml::xml_node<> * childNode = xmlutils::createNode(document, "Observation");
    const Observation * observation = mObservations.at(i).get();
    observation->writeToXml(document, childNode);
    observationsNode->append_node(childNode);
  }
  solutionNode->append_node(observationsNode);

  node->append_node(solutionNode);
}

void Solution::runSolution(double freqGHz, const Mesh * mesh)
{
  mSolver->runSolver(freqGHz, mesh, this);
}

void Solution::setSolver(std::unique_ptr<Solver> solver)
{
  mSolver = std::move(solver);
}
