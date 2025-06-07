#pragma once

#include "../../Interfaces/IUserRepository.hpp"
#include "../../Database/SQL.hpp"
#include "../../Database/DataTable.hpp"

class UserRepository : public IUserRepository
{
private:
        std::shared_ptr<SQL> Database;
public:
    explicit UserRepository(std::shared_ptr<SQL> Database);
    ~UserRepository();

    std::optional<std::vector<UserModel>>  ReadAll() override;
    std::optional<UserModel> ReadByEntry(const int&) override;
    bool Create(const UserModel&) override;
    bool Update(const UserModel&) override;
    bool Delete(const UserModel&) override;

    std::optional<std::vector<UserModel>> ReadByCode(const std::string) override;
    std::optional<std::vector<UserModel>> ReadByFirstName(const std::string) override;
    std::optional<std::vector<UserModel>> ReadByLastName(const std::string) override;
    std::optional<std::vector<UserModel>> ReadByPhone(const std::string) override;
    std::optional<std::vector<UserModel>> ReadByEmail(const std::string) override;
};
