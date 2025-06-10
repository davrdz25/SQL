#pragma once

#include <memory>
#include "../Repositories/User/UserRepository.hpp"
#include "../Interfaces/IUserRepository.hpp"

class UserService {
    public: 
        explicit UserService(std::shared_ptr<IUserRepository> userRepository);

        bool AddUser(const UserModel&);
        bool ModifyUser(const UserModel&);
        bool ModifyPassword(const int &, const std::string &, const std::string &);
        bool ExistsUser(const UserModel&);
        std::vector<UserModel> GetAllUsers();
        UserModel GetUserByEntry(const int&);

    private:
        std::shared_ptr<IUserRepository> userRepository;

};