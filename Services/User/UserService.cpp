#include "UserService.hpp"

UserService::UserService(std::shared_ptr<IUserRepository> userRepository) {};

bool UserService::AddUser(const UserModel &uNewUser)
{
    try
    {
        if(uNewUser.Code.length() == 0)
            throw std::invalid_argument("Code is empty");

        if(uNewUser.FirstName.length() == 0)
            throw std::invalid_argument("FirstName is empty");

        if(uNewUser.LastName.length() == 0)
            throw std::invalid_argument("LastName is empty");

        if(uNewUser.Email.length() == 0)
            throw std::invalid_argument("Email is empty");

        if(uNewUser.Phone.length() == 0)
            throw std::invalid_argument("Phone is empty");

        userRepository->Create(uNewUser);

        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
};
bool UserService::ModifyUser(const UserModel &uUser)
{
    try
    {
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
};
std::vector<UserModel> UserService::GetAllUsers()
{
    try
    {
        std::vector<UserModel> users;
        return users;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
};
UserModel UserService::GetUserByEntry(const int &iEntry)
{
    try
    {
        UserModel user;
        return user;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
};