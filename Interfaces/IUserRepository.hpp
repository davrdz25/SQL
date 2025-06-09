#pragma once
#include <optional>
#include <vector>

#include "../Interfaces/IBaseRepository.hpp"
#include "../Models/UserModel.hpp"

class IUserRepository : public IBaseRepository<UserModel>
{
    public: 
        ~IUserRepository() = default;
        virtual std::optional<std::vector<UserModel>> ReadByCode(const std::string) = 0;
        virtual std::optional<std::vector<UserModel>> ReadByFirstName(const std::string) = 0;
        virtual std::optional<std::vector<UserModel>> ReadByLastName(const std::string) = 0;
        virtual std::optional<std::vector<UserModel>> ReadByPhone(const std::string) = 0;
        virtual std::optional<std::vector<UserModel>> ReadByEmail(const std::string) = 0;
        virtual bool UpdatePassword(const int &, const std::string &) = 0;
};