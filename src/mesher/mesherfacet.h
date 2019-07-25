#ifndef MESHER_FACET_H
#define MESHER_FACET_H

#include "morfeus.h"
#include "mesherpolygon.h"

#include <vector>

class MesherFacet
{
public:
  MORFEUS_LIB_DECL MesherFacet();

public:
  MORFEUS_LIB_DECL const MesherPolygon & polygon(std::size_t index) const;
  MORFEUS_LIB_DECL std::size_t totalPolygons() const;

public:
  MORFEUS_LIB_DECL void addPolygon(const MesherPolygon & polygon);

private:
  std::vector<MesherPolygon> mPolygons;
};

inline const MesherPolygon & MesherFacet::polygon(std::size_t index) const
{
  return mPolygons.at(index);
}

inline std::size_t MesherFacet::totalPolygons() const
{
  return mPolygons.size();
}

#endif // MESHER_FACET_H
