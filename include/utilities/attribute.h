#ifndef ATTRIBUTE_H

#define ATTRIBUTE_H

#include <string>

template <typename T>
class Attribute_T
{
public:
	std::string m_Name;
	T m_Value;

	Attribute_T(const std::string& name, const T& value) : m_Name(name), m_Value(value) {}
};

#endif