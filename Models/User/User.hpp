// src/models/User.h
#pragma once
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include "SQL/SQL.hpp"
#include "DataTable/DataTable.hpp"
#include "GlobalEnvironment/GlobalEnvironment.hpp"

struct User
{
    int id;
    std::string name;
    std::string email;
    std::vector<User> User::ReadAll();
    std::optional<User> ReadById(int);
    bool Create(const std::string &, const std::string &);
    bool Create(const std::string &, const std::string &);
    bool Update(int, const std::string &, const std::string &);
    bool Delete(int);
};
