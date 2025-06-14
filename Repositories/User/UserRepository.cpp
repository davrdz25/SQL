#include "UserRepository.hpp"
#include "../../Utils/Hasher.hpp"

UserRepository::UserRepository(std::shared_ptr<SQL> Database) : Database(std::move(Database)) {}

std::optional<std::vector<UserModel>> UserRepository::ReadAll()
{
    try
    {
        DataTable dataTable;

        std::string sQuery = "SELECT [UserEntry], UserCode, FirstName, MiddleName, LastName, Phone, Email FROM Users";

        dataTable.Fill(Database->FetchResults(sQuery));

        if (dataTable.RowsCount() > 0)
        {
            std::vector<UserModel> users;

            for (int i = 0; i < dataTable.RowsCount(); i++)
            {
                users.push_back(UserModel{
                    std::stoi(dataTable[i]["UserEntry"]),
                    dataTable[i]["UserCode"],
                    dataTable[i]["FirstName"],
                    dataTable[i]["MiddleName"],
                    dataTable[i]["LastName"],
                    dataTable[i]["Phone"]});
            }

            return users;
        }

        return std::nullopt;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
};

std::optional<UserModel> UserRepository::ReadByEntry(const int &iEntry)
{
    try
    {
        DataTable dataTable;

        std::string sQuery = "SELECT [UserEntry], UserCode, FirstName, MiddleName, LastName, Phone, Email FROM Users WHERE [Entry] = ?";
        std::string sParam = std::to_string(iEntry);

        dataTable.Fill(Database->FetchPrepared(sQuery, sParam));

        if (dataTable.RowsCount() == 1)
        {
            UserModel user;

            user.Entry = std::stoi(dataTable[0]["UserEntry"]);
            user.Code = dataTable[0]["UserCode"];
            user.FirstName = dataTable[0]["FirstName"];
            user.MiddleName = dataTable[0]["MiddleName"];
            user.LastName = dataTable[0]["LastName"];
            user.Phone = dataTable[0]["Phone"];

            return user;
        }

        return std::nullopt;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
};


bool UserRepository::Create(const UserModel &uNewuser)
{
    try
    {
        std::vector<uint8_t> hashedPassword = Hasher::HashPassword(uNewuser.Password);

        std::string sQuery = "INSERT INTO Users (UserEntry, UserCode, FirstName, MiddleName, LastName, Email, Phone, Password) "
                             "VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

        std::vector<std::string> stringParams = {
            std::to_string(uNewuser.Entry),
            uNewuser.Code,
            uNewuser.FirstName,
            uNewuser.MiddleName,
            uNewuser.LastName,
            uNewuser.Email,
            uNewuser.Phone
        }; 

        std::vector<std::vector<uint8_t>> binaryParams = {
            hashedPassword
        };

        if (Database->RunPrepared(sQuery, stringParams, binaryParams))
            return true;

        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[CreateUser Exception] " << e.what() << '\n';
        throw std::runtime_error(e.what());
        return false;
    }
}

bool UserRepository::Update(const UserModel &)
{
    try
    {
        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
};

bool UserRepository::Delete(const UserModel &)
{
    try
    {
        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
};

std::optional<std::vector<UserModel>> UserRepository::ReadByCode(const std::string)
{
    try
    {
        return std::nullopt;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
};

std::optional<std::vector<UserModel>> UserRepository::ReadByFirstName(const std::string)
{
    try
    {
        
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
};

std::optional<std::vector<UserModel>> UserRepository::ReadByLastName(const std::string)
{
    try
    {
        
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
};

std::optional<std::vector<UserModel>> UserRepository::ReadByPhone(const std::string)
{
    try
    {
        
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
};

std::optional<std::vector<UserModel>> UserRepository::ReadByEmail(const std::string)
{
    try
    {
        
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
};

bool UserRepository::UpdatePassword(const int &userEntry, const std::string &newPassword)
{
    try
    {
        const std::string sQuery = "UPDATE Users SET Password = ? WHERE UserEntry = ?";
        const std::vector<std::string> vParams = {
            std::to_string(userEntry),
            newPassword
        };

        return true;

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
};
