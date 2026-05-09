#ifndef CSV_H

#define CSV_H

#include "csvmap.h"
#include "csvreader.h"
#include "../../tinyfiledialogs/tinyfiledialogs.h"

class CSV_T
{
private:
	std::string m_Filepath = "";
	std::vector<std::string> m_Headers;
	std::vector<CSVMap_T> m_Map;
	bool p_IsRead = false;
public:
	CSV_T() = default;
	CSV_T(const char* filepath) : m_Filepath(filepath) {}

	void read();

	const bool isread() const
	{
		return this->p_IsRead;
	}
	std::vector<std::string> header() const
	{
		return this->m_Headers;
	}
	std::vector<CSVMap_T> map() const
	{
		return this->m_Map;
	}
};

#endif