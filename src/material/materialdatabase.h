#ifndef MATERIALDATABASE_H
#define MATERIALDATABASE_H

#include "morfeus.h"
#include "morfeusobject.h"

#include <memory>
#include <vector>

class Material;

class MaterialDatabase
    : public MorfeusObject
{
public:
  MORFEUS_LIB_DECL MaterialDatabase();

public:
  MORFEUS_LIB_DECL std::size_t totalMaterials() const;

public:
  MORFEUS_LIB_DECL void addMaterial(std::unique_ptr<Material> material);
  MORFEUS_LIB_DECL Material * material(std::size_t index);
  MORFEUS_LIB_DECL const Material * material(std::size_t index) const;
  MORFEUS_LIB_DECL std::unique_ptr<Material> takeMaterial(std::size_t index);

protected:
  void doPrint(std::ostream & output, int tabPos) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

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

#endif // MATERIALDATABASE_H
