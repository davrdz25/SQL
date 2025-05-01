#pragma once
#include <string>
#include <vector>
#include <map>
#include "DataTable.hpp"

class ISQL {
public:
    virtual ~ISQL() = default;

    virtual bool Connect() = 0;
    virtual bool RunStatement(const std::string&) = 0;
    virtual std::vector<std::map<std::string, std::string>> FetchResults(const std::string&) = 0;

    virtual bool RunPrepared(const std::string& query, const std::vector<std::string>& params) = 0;
    virtual std::vector<std::map<std::string, std::string>> FetchPrepared(const std::string& query, const std::vector<std::string>& params) = 0;

};
