#include "UserService.hpp"

UserService::UserService(std::shared_ptr<IUserRepository> userRepository)
    : userRepository(std::move(userRepository)) {}

bool UserService::AddUser(const CreateUserDTO &uNewUser)
{
    try
    {
        if (uNewUser.Code.length() == 0)
            throw std::invalid_argument("Code is empty");

        if (uNewUser.FirstName.length() == 0)
            throw std::invalid_argument("FirstName is empty");

        if (uNewUser.LastName.length() == 0)
            throw std::invalid_argument("LastName is empty");

        if (uNewUser.Email.length() == 0)
            throw std::invalid_argument("Email is empty");

        if (uNewUser.Phone.length() == 0)
            throw std::invalid_argument("Phone is empty");

        if (userRepository->Create(uNewUser))
            return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        throw std::runtime_error(e.what());
    }
};

bool UserService::ModifyUser(const UserDTO &uUser)
{
    try
    {
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
};
std::vector<UserPublicModel> UserService::GetAllUsers()
{
    try
    {
        std::vector<UserPublicModel> users;
        return users;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
};
UserPublicModel UserService::GetUserByEntry(const int &iEntry)
{
    try
    {
        UserPublicModel user;
        DataTable data;

        data = userRepository->ReadByEntry(iEntry);

        if (data.RowsCount() == 0)
            return user;

        if (data.RowsCount() > 1)
            throw std::runtime_error("Inconstence retreiving Users, not exists or exists more than one");

        user.Entry = std::stoi(data[0]["Entry"].value());
        user.Code = data[0]["Code"].value();
        user.FirstName = data[0]["FirstName"].value();
        user.MiddleName = data[0]["MiddleName"].value();
        user.LastName = data[0]["LastName"].value();
        user.SecondLastName = data[0]["SecondLastName"].value();
        user.Email = data[0]["Email"].value();
        user.Phone = data[0]["Phone"].value();

        return user;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
};

bool UserService::ModifyPassword(const int &userEntry, const std::string &oldPassword, const std::string &newPassword)
{
    try
    {
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
};

std::vector<UserPublicModel> UserService::SearchUsers(const UserDTO &filter)
{
    const std::vector<UserPublicModel> users;

    return users;
};

AuthPayloadModel UserService::CreateUserSession(const UserSessionDTO &dto)
{
    UserPublicModel user;
    const UserSessionDTO authPayload;

    const AuthPayloadModel payload{
        "",
        user};

    return payload;
};

UserPublicModel UserService::GetUserByCode(const std::string &code)
{
    try
    {
        UserPublicModel user;
        DataTable data;

        data = userRepository->ReadByCode(code);

        if (data.RowsCount() == 0)
            return user;

        if (data.RowsCount() > 1)
            throw std::runtime_error("Inconstence retreiving Users, not exists or exists more than one");

        user.Entry = std::stoi(data[0]["UserEntry"].value());
        user.Code = data[0]["UserCode"].value();
        user.FirstName = data[0]["FirstName"].value();
        user.MiddleName = data[0]["MiddleName"].value_or("");
        user.LastName = data[0]["LastName"].value();
        user.SecondLastName = data[0]["SecondLastName"].value_or("");
        user.Email = data[0]["Email"].value();
        user.Phone = data[0]["Phone"].value();

        return user;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("[GetUserByCode Exception] ") + " " + e.what());
    }
};

UserPublicModel UserService::GetUserByPhone(const std::string &phone)
{
    try
    {
        UserPublicModel user;
        DataTable data;

        data = userRepository->ReadByPhone(phone);

        if (data.RowsCount() == 0)
            return user;

        if (data.RowsCount() > 1)
            throw std::runtime_error("Inconstence retreiving Users, not exists or exists more than one");

        user.Entry = std::stoi(data[0]["Entry"].value());
        user.Code = data[0]["Code"].value();
        user.FirstName = data[0]["FirstName"].value();
        user.MiddleName = data[0]["MiddleName"].value();
        user.LastName = data[0]["LastName"].value();
        user.SecondLastName = data[0]["SecondLastName"].value();
        user.Email = data[0]["Email"].value();
        user.Phone = data[0]["Phone"].value();

        return user;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
};

UserPublicModel UserService::GetUserByEmail(const std::string &email)
{
    try
    {
        UserPublicModel user;
        DataTable data;

        data = userRepository->ReadByEmail(email);

        if (data.RowsCount() == 0)
            return user;

        if (data.RowsCount() > 1)
            throw std::runtime_error("Inconstence retreiving Users, not exists or exists more than one");

        user.Entry = std::stoi(data[0]["Entry"].value());
        user.Code = data[0]["Code"].value();
        user.FirstName = data[0]["FirstName"].value();
        user.MiddleName = data[0]["MiddleName"].value();
        user.LastName = data[0]["LastName"].value();
        user.SecondLastName = data[0]["SecondLastName"].value();
        user.Email = data[0]["Email"].value();
        user.Phone = data[0]["Phone"].value();

        return user;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    };
};