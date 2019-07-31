#ifndef MORFEUS_XML_XMLUTILS_H
#define MORFEUS_XML_XMLUTILS_H

#include "rapidxml.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace xmlutils
{
  rapidxml::xml_node<> * createNode(rapidxml::xml_document<> & document, const std::string & name);

  template <typename T>
  rapidxml::xml_node<> * createNode(rapidxml::xml_document<> & document, const std::string & name, const T & value);

  template <typename T>
  T parseValue(const rapidxml::xml_node<> * node);

  template <typename T>
  T readAttribute(rapidxml::xml_node<> * node, const std::string & name, T defaultValue = T());

  template <typename T>
  void writeAttribute(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node, const std::string & name, const T & value);

  template <typename T>
  void printValue(std::ostream & output, int tabPos, const std::string & name, const T & value);

  //! Creates a node with no value in a rapidxml document. Usually used for nodes with children.
  inline rapidxml::xml_node<> * createNode(rapidxml::xml_document<> & document, const std::string & name)
  {
    std::string value;
    return createNode(document, name, value);
  }

  //! Creates a node with a value in a rapidxml document. Usually used for nodes that do not have children.
  template <typename T>
  inline rapidxml::xml_node<> * createNode(rapidxml::xml_document<> & document, const std::string & name, const T & value)
  {
    std::ostringstream oss;
    oss << value;

    char * typeString = document.allocate_string(name.c_str());
    char * valueString = document.allocate_string(oss.str().c_str());
    rapidxml::xml_node<> * node = document.allocate_node(rapidxml::node_element, typeString);
    node->value(valueString);
    return node;
  }

  //! Creates a node with a value in a rapidxml document. Usually used for nodes that do not have children.
  template <>
  inline rapidxml::xml_node<> * createNode<bool>(rapidxml::xml_document<> & document, const std::string & name, const bool & value)
  {
    std::ostringstream oss;
    oss << ( (value) ? "true" : "false");

    char * typeString = document.allocate_string(name.c_str());
    char * valueString = document.allocate_string(oss.str().c_str());
    rapidxml::xml_node<> * node = document.allocate_node(rapidxml::node_element, typeString);
    node->value(valueString);
    return node;
  }

  //! Parse a value from the specified node
  template <typename T>
  inline T parseValue(const rapidxml::xml_node<> * node)
  {
    return (node != nullptr) ? node->value() : T();
  }

  template <>
  inline bool parseValue<bool>(const rapidxml::xml_node<> * node)
  {
    if (node != nullptr) {
      std::string val(node->value());
      std::transform(val.begin(), val.end(), val.begin(), tolower);
      return (val == "true");
    }
    return false;
  }

  template <>
  inline uint16_t parseValue<uint16_t>(const rapidxml::xml_node<> * node)
  {
    return (node != nullptr) ? std::stoi(node->value()) : 0;
  }

  template <>
  inline int16_t parseValue<int16_t>(const rapidxml::xml_node<> * node)
  {
    return (node != nullptr) ? std::stoi(node->value()) : 0;
  }

  template <>
  inline uint32_t parseValue<uint32_t>(const rapidxml::xml_node<> * node)
  {
    return (node != nullptr) ? std::stoul(node->value()) : 0;
  }

  template <>
  inline int32_t parseValue<int32_t>(const rapidxml::xml_node<> * node)
  {
    return (node != nullptr) ? std::stol(node->value()) : 0;
  }

  template <>
  inline uint64_t parseValue<uint64_t>(const rapidxml::xml_node<> * node)
  {
    return (node != nullptr) ? std::stoull(node->value()) : 0;
  }

  template <>
  inline int64_t parseValue<int64_t>(const rapidxml::xml_node<> * node)
  {
    return (node != nullptr) ? std::stoll(node->value()) : 0;
  }

  template <>
  inline float parseValue<float>(const rapidxml::xml_node<> * node)
  {
    return (node != nullptr) ? std::stof(node->value()) : 0.0;
  }

  template <>
  inline double parseValue<double>(const rapidxml::xml_node<> * node)
  {
    return (node != nullptr) ? std::stod(node->value()) : 0.0;
  }

  template <>
  inline long double parseValue<long double>(const rapidxml::xml_node<> * node)
  {
    return (node != nullptr) ? std::stold(node->value()) : 0.0;
  }

  inline void printHeader(std::ostream & output, int tabPos, const std::string & name)
  {
    output << std::setfill(' ') << std::setw(tabPos) << " " << name << std::endl;
  }

  template <typename T>
  inline void printValue(std::ostream & output, int tabPos, const std::string & name, const T & value)
  {
    output << std::setfill(' ') << std::setw(tabPos) << " " << name << " " << value << std::endl;
  }

  template <>
  inline void printValue<bool>(std::ostream & output, int tabPos, const std::string & name, const bool & value)
  {
    std::string valueStr = (value) ? "true" : "false";
    output << std::setfill(' ') << std::setw(tabPos) << " " << name << " " << valueStr << std::endl;
  }

  template <>
  inline int32_t readAttribute<int32_t>(rapidxml::xml_node<> * node, const std::string & name, int32_t defaultValue)
  {
    int32_t value = defaultValue;
    rapidxml::xml_attribute<> * attr = node->first_attribute(name.c_str(), 0, false);
    if (attr != nullptr) {
      value = stoi(std::string(attr->value()));
    }
    return value;
  }

  template <>
  inline std::size_t readAttribute<std::size_t>(rapidxml::xml_node<> * node, const std::string & name, std::size_t defaultValue)
  {
    std::size_t value = defaultValue;
    rapidxml::xml_attribute<> * attr = node->first_attribute(name.c_str(), 0, false);
    if (attr != nullptr) {
      value = stoul(std::string(attr->value()));
    }
    return value;
  }

  template <>
  inline double readAttribute<double>(rapidxml::xml_node<> * node, const std::string & name, double defaultValue)
  {
    double value = defaultValue;
    rapidxml::xml_attribute<> * attr = node->first_attribute(name.c_str(), 0, false);
    if (attr != nullptr) {
      value = stod(std::string(attr->value()));
    }
    return value;
  }

  template <>
  inline std::string readAttribute<std::string>(rapidxml::xml_node<> * node, const std::string & name, std::string defaultValue)
  {
    std::string s = defaultValue;
    rapidxml::xml_attribute<> * attr = node->first_attribute(name.c_str(), 0, false);
    if (attr != nullptr) {
      s = attr->value();
    }
    return s;
  }

  //! Writes an attribute to a rapidxml node with the specified value.
  template <typename T>
  inline void writeAttribute(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node, const std::string & name, const T & value)
  {
    std::ostringstream oss;
    oss << value;

    char * valueString = document.allocate_string(oss.str().c_str());
    char * nameString = document.allocate_string(name.c_str());
    rapidxml::xml_attribute<> * attr = document.allocate_attribute(nameString, valueString);
    node->append_attribute(attr);
  }

  template <>
  inline void writeAttribute<bool>(rapidxml::xml_document<> & document, rapidxml::xml_node<> * node, const std::string & name, const bool & value)
  {
    std::ostringstream oss;
    oss << ( (value) ? "true" : "false");

    char * valueString = document.allocate_string(oss.str().c_str());
    char * nameString = document.allocate_string(name.c_str());
    rapidxml::xml_attribute<> * attr = document.allocate_attribute(nameString, valueString);
    node->append_attribute(attr);
  }

}
#endif // MORFEUS_XML_XMLUTILS_H
