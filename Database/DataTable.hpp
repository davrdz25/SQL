#pragma once

#include <string>
#include <iostream>
#include <unordered_map>
#include <optional>
#include <vector>
#include <stdexcept>

class DataTable
{
public:
    using Value = std::optional<std::string>;

    class Row
    {
    public:
        Value &operator[](const std::string &column);
        const Value &operator[](const std::string &column) const;

        void Set(const std::string &column, const Value &value);
        void SetColumns(const std::vector<std::string> &columns);

    private:
        std::unordered_map<std::string, Value> values;
        std::vector<std::string> expectedColumns;
    };

    void SetColumns(const std::vector<std::string> &columns);
    void Fill(const std::vector<Row> &newData);
    Row &operator[](int index);
    const Row &operator[](int index) const;
    int RowsCount() const;

private:
    std::vector<std::string> columns;
    std::vector<Row> data;
};
