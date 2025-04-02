#include "Dataset.hpp"

void Dataset::setData(const std::vector<std::map<std::string, std::string>> &newData)
{
    data = newData;
}

std::map<std::string, std::string> &Dataset::operator[](int index)
{
    if (index < 0 || index >= data.size())
    {
        throw std::out_of_range("Índice fuera de rango");
    }
    return data[index];
}

int Dataset::size() const
{
    return data.size();
}
