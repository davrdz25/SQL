#include "DataTable.hpp"

void DataTable::SetColumns(const std::vector<std::string> &cols)
{
    columns = cols;
}

void DataTable::Fill(const std::vector<Row> &newData)
{
    data.insert(data.end(), newData.begin(), newData.end());
}

DataTable::Row &DataTable::operator[](int index)
{
    if (index < 0 || index >= static_cast<int>(data.size()))
    {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

const DataTable::Row &DataTable::operator[](int index) const
{
    if (index < 0 || index >= static_cast<int>(data.size()))
    {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

int DataTable::RowsCount() const
{
    return static_cast<int>(data.size());
}

void DataTable::Row::Set(const std::string &column, const Value &value)
{
    values[column] = value;
}

void DataTable::Row::SetColumns(const std::vector<std::string> &columns)
{
    expectedColumns = columns;
    for (const auto &col : columns)
    {
        if (values.find(col) == values.end())
        {
            values[col] = std::nullopt;
        }
    }
}

DataTable::Value &DataTable::Row::operator[](const std::string &column)
{
    try
    {
        if (values.find(column) == values.end())
            throw std::runtime_error("Column '" + column + "' not found in data table");

        return values[column];
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        throw std::runtime_error(e.what());
    }
}

const DataTable::Value &DataTable::Row::operator[](const std::string &column) const
{
    try
    {
        auto it = values.find(column);

        if (it == values.end())
            throw std::runtime_error("Column '" + column + "' not found data table");
        
        return it->second;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        throw std::runtime_error(e.what());
    }
}
