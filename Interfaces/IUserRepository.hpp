#pragma once
#include <optional>
#include <vector>

#include "../DTOs/UserDTO.hpp"
#include "../DTOs/CreateUserDTO.hpp"
#include "../Models/UserPublicModel.hpp"
#include "../Database/DataTable.hpp"

class IUserRepository
{
    public: 
        ~IUserRepository() = default;
        virtual bool Create(const CreateUserDTO&) = 0;
        virtual bool Update(const UserDTO &) = 0;
        virtual bool Delete(const UserDTO &) = 0;
        virtual DataTable ReadAll() = 0;
        virtual DataTable ReadByEntry(const int &) = 0;
        virtual DataTable ReadByCode(const std::string &) = 0;
        virtual DataTable ReadByFirstName(const std::string &) = 0;
        virtual DataTable ReadByLastName(const std::string &) = 0;
        virtual DataTable ReadByPhone(const std::string &) = 0;
        virtual DataTable ReadByEmail(const std::string &) = 0;
        virtual bool UpdateUserSequence() = 0;
        virtual bool GetCurrentSequence() = 0;
        //virtual bool ValidatePassword() = 0;
        virtual bool UpdatePassword(const int &, const std::string &) = 0;
};