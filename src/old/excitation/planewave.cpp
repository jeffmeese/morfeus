#include "excitation/planewave.h"

#include "mesh/face.h"
#include "mesh/mesh.h"

Planewave::Planewave(std::size_t id)
  : Excitation(id)
  , mFieldPolarization(0.0)
  , mPhiIncident(0.0)
  , mThetaIncident(0.0)
{
}

Planewave::Planewave(size_t id, double fieldPolarization, double thetaIncident, double phiIncident)
  : Excitation(id)
  , mFieldPolarization(fieldPolarization)
  , mPhiIncident(phiIncident)
  , mThetaIncident(thetaIncident)
{
}

void Planewave::excite(const Mesh &mesh, double freq, dcomplex_array & rhs) const
{
	std::vector<std::unique_ptr<Face>> boundaryFaces = mesh.getBoundaryFaces();

	for (size_t i = 0; i < boundaryFaces.size(); i++) {
		Face * boundaryFace = boundaryFaces.at(i).get();
		if (boundaryFace->position() == Face::Position::Top) {
			for (size_t j = 0; j < boundaryFace->totalEdges(); j++) {
				Edge * edge = mesh.getEdge(boundaryFace->globalEdge(j));
				int unknown = edge->unknown();
				if (unknown > 0) {
					dcomplex result = boundaryFace->computePlanewaveEntry(j, freq, this, mesh);
					rhs[unknown-1] += result;
				}
			}
		}
	}
}

double Planewave::fieldPolarization() const
{
  return mFieldPolarization;
}

double Planewave::phiIncident() const
{
  return mPhiIncident;
}

double Planewave::thetaIncident() const
{
  return mThetaIncident;
}

void Planewave::setFieldPolarization(double fieldPolarization)
{
  mFieldPolarization = fieldPolarization;
}

void Planewave::setThetaIncident(double theta)
{
  mThetaIncident = theta;
}

void Planewave::setPhiIncident(double phi)
{
  mPhiIncident = phi;
}
