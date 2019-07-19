#include "mesh/meshsection.h"

MeshSection::MeshSection()
{
  mElementCode = 0;
  mElementType = 0;
  mEpsId = 0;
  mMuId = 0;
  mLayer = 0;
}

MeshSection::MeshSection(std::size_t layer, std::size_t elementCode, std::size_t epsId, std::size_t muId, std::size_t elementType)
{
  mElementCode = elementCode;
  mEpsId = epsId;
  mMuId = muId;
  mLayer = layer;
  mElementType = elementType;
}

std::size_t MeshSection::elementCode() const
{
  return mElementCode;
}

std::size_t MeshSection::elementType() const
{
  return mElementType;
}

std::size_t MeshSection::epsId() const
{
  return mEpsId;
}

std::size_t MeshSection::layer() const
{
  return mLayer;
}

std::size_t MeshSection::muId() const
{
  return mMuId;
}

void MeshSection::setElementCode(size_t elementCode)
{
  mElementCode = elementCode;
}

void MeshSection::setElementType(std::size_t elementType)
{
  mElementType = elementType;
}

void MeshSection::setEpsId(std::size_t epsId)
{
  mEpsId = epsId;
}

void MeshSection::setLayer(std::size_t layer)
{
  mLayer = layer;
}

void MeshSection::setMuId(std::size_t muId)
{
  mMuId = muId;
}
