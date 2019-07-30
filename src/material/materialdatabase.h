#ifndef MATERIALDATABASE_H
#define MATERIALDATABASE_H

#include "morfeus.h"
#include "morfeusobject.h"
#include "rapidxml.hpp"
#include "xmlutils.h"

#include <memory>
#include <vector>

namespace Morfeus {

class Material;

class MaterialDatabase
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL MaterialDatabase();
  MORFEUS_LIB_DECL MaterialDatabase(const std::string & id);

public:
  MORFEUS_LIB_DECL std::size_t totalMaterials() const;

public:
  MORFEUS_LIB_DECL void addMaterial(std::unique_ptr<Material> material);
  MORFEUS_LIB_DECL Material * material(std::size_t index);
  MORFEUS_LIB_DECL const Material * material(std::size_t index) const;
  MORFEUS_LIB_DECL void print(std::ostream & output, int tabPos = 0) const;
  MORFEUS_LIB_DECL void print(int tabPos = 0) const;
  MORFEUS_LIB_DECL void readFromXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node);
  MORFEUS_LIB_DECL void writeToXml(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const;
  MORFEUS_LIB_DECL std::unique_ptr<Material> takeMaterial(std::size_t index);
  MORFEUS_LIB_DECL friend std::ostream & operator<<(std::ostream & output, const Material & object);

private:
  typedef std::unique_ptr<Material> MaterialPtr;
  typedef std::vector<MaterialPtr> MaterialVector;

private:
  MaterialVector mMaterials;
};

inline Material * MaterialDatabase::material(std::size_t index)
{
  return mMaterials.at(index).get();
}

inline const Material * MaterialDatabase::material(std::size_t index) const
{
  return mMaterials.at(index).get();
}

inline std::size_t MaterialDatabase::totalMaterials() const
{
  return mMaterials.size();
}

}

#endif // MATERIALDATABASE_H
