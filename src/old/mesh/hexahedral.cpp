#include "mesh/hexahedral.h"
#include "mesh/face.h"
#include "mesh/quadrilateral.h"

Hexahedral::Hexahedral(std::size_t id)
	: Element(id, 8, 12, 6)
{

}

dcomplex Hexahedral::computeMatrixEntry(const Material * eps, const Material * mu, const Mesh & mesh, double freq, std::size_t source, std::size_t test) const
{
	dcomplex matrixEntry(0.0,0.0);
	return matrixEntry;
}

std::unique_ptr<Element> Hexahedral::copy(size_t newId) const
{
	auto newPrism(std::make_unique<Hexahedral>(newId));
	return std::move(newPrism);
}

std::unique_ptr<Face> Hexahedral::createFace(size_t localFace) const
{
	std::unique_ptr<Face> newFace(new Quadrilateral);
	return std::move(newFace);
}

void Hexahedral::getEdgeNodes(std::size_t index, std::size_t &node1, std::size_t &node2) const
{
}
