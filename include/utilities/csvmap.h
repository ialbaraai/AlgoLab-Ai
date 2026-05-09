#ifndef CSVDATA_H

#define CSVDATA_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

template <typename T>
class convert_result
{
private:
    bool m_IsSuccess;
    T m_Value;
public:
    convert_result(const T& value, const bool success)
    {
        this->m_Value = value;
        this->m_IsSuccess = success;
    }

    bool success()
    {
        return this->m_IsSuccess;
    }

    T value()
    {
        return this->m_Value;
    }
};

class CSVMap_T
{
public:
    std::vector<std::string> m_Row;

    std::string at(size_t index)
    {
        if (index < m_Row.size()) return m_Row[index];
        return "";
    }

    template <typename T>
    convert_result<T> convert(size_t index)
    {
        if (index >= m_Row.size())
            return convert_result<T>(T{}, false);

        const std::string& cell = m_Row[index];

        try
        {
            if constexpr (std::is_same_v<T, int>)
                return convert_result<T>(std::stoi(cell), true);
            else if constexpr (std::is_same_v<T, long>)
                return convert_result<T>(std::stol(cell), true);
            else if constexpr (std::is_same_v<T, long long>)
                return convert_result<T>(std::stoll(cell), true);
            else if constexpr (std::is_same_v<T, float>)
                return convert_result<T>(std::stof(cell), true);
            else if constexpr (std::is_same_v<T, double>)
                return convert_result<T>(std::stod(cell), true);
            else if constexpr (std::is_same_v<T, long double>)
                return convert_result<T>(std::stold(cell), true);
            else if constexpr (std::is_same_v<T, std::string>)
                return convert_result<T>(cell, true);
            else
            {
                T result{};
                std::istringstream ss(cell);
                if (ss >> result && ss.eof())
                    return convert_result<T>(result, true);
                else
                    return convert_result<T>(T{}, false);
            }
        }
        catch (const std::invalid_argument&)
        {
            return convert_result<T>(T{}, false);
        }
        catch (const std::out_of_range&)
        {
            return convert_result<T>(T{}, false);
        }
    }
};

#endif