#include "DataTable.hpp"

void DataTable::setData(const std::vector<std::map<std::string, std::string>> &newData)
{
    data = newData;
}

std::map<std::string, std::string> &DataTable::operator[](int index)
{
    if (index < 0 || index >= data.size())
    {
        throw std::out_of_range("Índice fuera de rango");
    }
    return data[index];
}

int DataTable::size() const
{
    return data.size();
}
