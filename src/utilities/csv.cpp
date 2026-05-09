#include "../../include/utilities/csv.h"

void CSV_T::read()
{
    try
    {
        rapidcsv::Document csv(this->m_Filepath);

        this->m_Map.clear();
        this->m_Headers.clear();

        size_t rows = csv.GetRowCount();
        size_t cols = csv.GetColumnCount();

        this->m_Headers = csv.GetColumnNames();

        for (size_t i = 0; i < rows; ++i)
        {
            CSVMap_T map;

            for (size_t j = 0; j < cols; ++j)
            {
                map.m_Row.push_back(csv.GetCell<std::string>(j, i));
            }

            this->m_Map.push_back(map);
        }

        this->p_IsRead = true;
    }
    catch (std::exception& err)
    {
        std::cerr << "Error while reading CSV file: " << err.what() << '\n';

        return;
    }
}