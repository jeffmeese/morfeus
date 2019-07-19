#ifndef MESHSECTION_H
#define MESHSECTION_H

#include "morfeus.h"

#include <cstddef>

class MeshSection
{
public:
  MORFEUS_LIB_DECL MeshSection();
  MORFEUS_LIB_DECL MeshSection(std::size_t layer, std::size_t elementCode, std::size_t epsId, std::size_t muId, std::size_t elementType);

public:
  MORFEUS_LIB_DECL std::size_t elementCode() const;
  MORFEUS_LIB_DECL std::size_t elementType() const;
  MORFEUS_LIB_DECL std::size_t epsId() const;
  MORFEUS_LIB_DECL std::size_t layer() const;
  MORFEUS_LIB_DECL std::size_t muId() const;
  MORFEUS_LIB_DECL void setElementCode(std::size_t elementCode);
  MORFEUS_LIB_DECL void setElementType(std::size_t elementType);
  MORFEUS_LIB_DECL void setEpsId(std::size_t epsId);
  MORFEUS_LIB_DECL void setLayer(std::size_t layer);
  MORFEUS_LIB_DECL void setMuId(std::size_t muId);

private:
  std::size_t mElementCode;
  std::size_t mElementType;
  std::size_t mEpsId;
  std::size_t mLayer;
  std::size_t mMuId;
};

#endif // MESHSECTION_H
