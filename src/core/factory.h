#ifndef FACTORY_H
#define FACTORY_H

#include <boost/function.hpp>
#include <boost/container/flat_map.hpp>
#include <utility>

#include <string>

template <class AbstractProduct, class IdType=std::string,
          class MakeType=boost::function<typename std::remove_pointer<AbstractProduct>::type*()> >
class Factory
{
  using abstract_type = typename std::remove_pointer<AbstractProduct>::type;

public:
  void registerFactory(IdType type, const MakeType & make)
  {
    mFactoryMap.insert(std::make_pair(type, make));
  }

  template <class ...args>
  abstract_type * create(IdType type, args&&... a) const
  {
    auto itr = mFactoryMap.find(type);
    if (itr != mFactoryMap.end()) {
      return itr->second(std::forward<args>(a)...);
    }
    return nullptr;
  }
private:
  boost::container::flat_map<IdType, MakeType> mFactoryMap;
};

#endif // FACTORY_H
