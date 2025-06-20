#pragma once

#include <memory>
#include "../Repositories/User/UserRepository.hpp"
#include "../Interfaces/IUserRepository.hpp"
#include "../Models/UserPublicModel.hpp"
#include "../Models/AuthPayloadModel.hpp"
#include "../DTOs/UserSessionDTO.hpp"
class UserService {
    public: 
        explicit UserService(std::shared_ptr<IUserRepository> userRepository);

        bool AddUser(const CreateUserDTO&);
        bool ModifyUser(const UserDTO&);
        bool ModifyPassword(const int &, const std::string &, const std::string &);
        bool ExistsUser(const UserDTO&);
        std::vector<UserPublicModel> GetAllUsers();
        std::vector<UserPublicModel> SearchUsers(const UserDTO &);
        UserPublicModel GetUserByEntry(const int&);
        UserPublicModel GetUserByCode(const std::string&);
        UserPublicModel GetUserByPhone(const std::string&);
        UserPublicModel GetUserByEmail(const std::string&);
        AuthPayloadModel CreateUserSession(const UserSessionDTO &);
        

    private:
        std::shared_ptr<IUserRepository> userRepository;

};