#ifndef MORFEUS_MODEL_MODEL_H
#define MORFEUS_MODEL_MODEL_H

#include "core/morfeusobject.h"

#include <memory>
#include <vector>

namespace morfeus {
  namespace model {
    namespace geometry {
      class Part;
    }
    namespace media {
      class Medium;
    }
    namespace ports {
      class Port;
    }
  }
}

namespace morfeus {
namespace model {

class Model
    : public core::MorfeusObject
{
public:
  MORFEUS_LIB_DECL Model();
  MORFEUS_LIB_DECL virtual ~Model();
  
public:
  MORFEUS_LIB_DECL void addMedium(std::unique_ptr<media::Medium> media);
  MORFEUS_LIB_DECL void addPart(std::unique_ptr<geometry::Part> part);
  MORFEUS_LIB_DECL void addPort(std::unique_ptr<ports::Port> port);
  MORFEUS_LIB_DECL media::Medium * medium(std::size_t index);
  MORFEUS_LIB_DECL const media::Medium * medium(std::size_t index) const;
  MORFEUS_LIB_DECL geometry::Part * part(std::size_t index);
  MORFEUS_LIB_DECL const geometry::Part * part(std::size_t index) const;
  MORFEUS_LIB_DECL ports::Port * port(std::size_t index);
  MORFEUS_LIB_DECL const ports::Port * port(std::size_t index) const;
  MORFEUS_LIB_DECL std::size_t totalMediums() const;
  MORFEUS_LIB_DECL std::size_t totalParts() const;
  MORFEUS_LIB_DECL std::size_t totalPorts() const;

protected:
  void doPrint(std::ostream & output, int tabPos = 0) const override;
  void doXmlRead(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) override;
  void doXmlWrite(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node) const override;

private:
  typedef std::unique_ptr<media::Medium> MediumPtr;
  typedef std::unique_ptr<geometry::Part> PartPtr;
  typedef std::unique_ptr<ports::Port> PortPtr;
  typedef std::vector<MediumPtr> MediumVector;
  typedef std::vector<PartPtr> PartVector;
  typedef std::vector<PortPtr> PortVector;

private:
  MediumVector mMedia;
  PartVector mParts;
  PortVector mPorts;
};

inline media::Medium * Model::medium(std::size_t index)
{
  return mMedia.at(index).get();
}

inline const media::Medium * Model::medium(std::size_t index) const
{
  return mMedia.at(index).get();
}

inline geometry::Part * Model::part(std::size_t index)
{
  return mParts.at(index).get();
}

inline const geometry::Part * Model::part(std::size_t index) const
{
  return mParts.at(index).get();
}

inline ports::Port * Model::port(std::size_t index)
{
  return mPorts.at(index).get();
}

inline const ports::Port * Model::port(std::size_t index) const
{
  return mPorts.at(index).get();
}

inline std::size_t Model::totalMediums() const
{
  return mMedia.size();
}

inline std::size_t Model::totalParts() const
{
  return mParts.size();
}

inline std::size_t Model::totalPorts() const
{
  return mPorts.size();
}

}
}

#endif // MORFEUS_MODEL_MODEL_H
