#include "UserRepository.hpp"
#include "../../Utils/Hasher.hpp"

UserRepository::UserRepository(std::shared_ptr<SQL> Database) : Database(std::move(Database)) {}

DataTable UserRepository::ReadAll()
{
    try
    {
        DataTable dataTable;

        std::string sQuery = "SELECT [UserEntry], UserCode, FirstName, MiddleName, LastName, SecondLastName, Phone, Email FROM Users";

        dataTable = Database->FetchResults(sQuery);

        return dataTable;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
};

DataTable UserRepository::ReadByEntry(const int &iEntry)
{
    try
    {
        DataTable dataTable;

        std::string sQuery = "SELECT [UserEntry], UserCode, FirstName, MiddleName, LastName, SecondLastName, Phone, Email FROM Users WHERE [Entry] = ?";

        dataTable = Database->FetchPrepared(sQuery, std::to_string(iEntry));

        return dataTable;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
};

bool UserRepository::Create(const CreateUserDTO &uNewuser)
{
    //TODO: Support NULL values 
    try
    {
        Database->BeginTransaction();

        GetCurrentSequence();

        std::vector<uint8_t> hashedPassword = Hasher::HashPassword(uNewuser.Password);

        std::string sQuery = "INSERT INTO Users (UserEntry, UserCode, FirstName, MiddleName, LastName, SecondLastName, Email, Phone, Password) "
                             "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";

        std::vector<std::optional<std::string>> stringParams = {
            std::to_string(_UserEntry),
            uNewuser.Code,
            uNewuser.FirstName,
            uNewuser.MiddleName,   
            uNewuser.LastName,
            uNewuser.SecondLastName,
            uNewuser.Email,
            uNewuser.Phone
        };

        std::vector<std::vector<uint8_t>> binaryParams = {
            hashedPassword
        };

        if (!Database->RunPrepared(sQuery, stringParams, binaryParams))
        {
            Database->RollbackTransaction();
            throw std::runtime_error("Error creating user");
        }

        if (!UpdateUserSequence())
        {
            Database->RollbackTransaction();
            throw std::runtime_error("Error updating sequence");
        }

        Database->CommitTransaction();

        return true;
    }
    catch (const std::exception &e)
    {
        Database->RollbackTransaction();
        throw std::runtime_error(std::string("[CreateUser Exception] ") + " " + e.what());        
    }
}

bool UserRepository::UpdateUserSequence()
{
    try
    {
        const std::string sQuery = "UPDATE Sequences SET User_Seq = User_Seq + 1";

        if (Database->RunStatement(sQuery))
            return true;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("[UpdateUserSequence Exception] ") + " " + e.what());        
    }
};

bool UserRepository::GetCurrentSequence()
{
    try
    {
        const std::string sQuery = "SELECT ISNULL(User_Seq, 0) + 1 User_Seq FROM Sequences";

        DataTable data = Database->FetchResults(sQuery);

        std::cout << "Sequence " << data[0]["User_Seq"].value() << std::endl;

        if (data.RowsCount() == 1)
            _UserEntry = std::stoi(data[0]["User_Seq"].value());
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("[GetCurrentSequence Exception] ") + " " + e.what());
    }
};


bool UserRepository::Update(const UserDTO &)
{
    try
    {
        return false;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("[GetCurrentSequence Exception] ") + " " + e.what());
    }
};

bool UserRepository::Delete(const UserDTO &)
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

DataTable UserRepository::ReadByCode(const std::string &Code)
{
    try
    {
        DataTable dataTable;

        std::string sQuery = "SELECT [UserEntry], UserCode, FirstName, MiddleName, LastName, SecondLastName, Phone, Email FROM Users WHERE UserCode = ?";

        dataTable = Database->FetchPrepared(sQuery, Code);

        return dataTable;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("[ReadByCode Exception] ") + " " + e.what());
    }
};

DataTable UserRepository::ReadByFirstName(const std::string &firstName)
{
    try
    {
        DataTable dataTable;

        std::string sQuery = "SELECT [UserEntry], UserCode, FirstName, MiddleName, LastName, SecondLastName, Phone, Email FROM Users WHERE FirstName = ?";

        dataTable = Database->FetchPrepared(sQuery, firstName);

        return dataTable;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
};

DataTable UserRepository::ReadByLastName(const std::string &lastName)
{
    try
    {
        DataTable dataTable;

        std::string sQuery = "SELECT [UserEntry], UserCode, FirstName, MiddleName, LastName, SecondLastName, Phone, Email FROM Users WHERE LastName = ?";

        dataTable = Database->FetchPrepared(sQuery, lastName);

        return dataTable;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
};

DataTable UserRepository::ReadByPhone(const std::string &phone)
{
    try
    {
        DataTable dataTable;

        std::string sQuery = "SELECT [UserEntry], UserCode, FirstName, MiddleName, LastName, SecondLastName, Phone, Email FROM Users WHERE Phone = ?";

        dataTable = Database->FetchPrepared(sQuery, phone);

        return dataTable;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
};

DataTable UserRepository::ReadByEmail(const std::string &email)
{
    try
    {
        DataTable dataTable;

        std::string sQuery = "SELECT [UserEntry], UserCode, FirstName, MiddleName, LastName, SecondLastName, Phone, Email FROM Users WHERE Email = ?";

        dataTable = Database->FetchPrepared(sQuery, email);

        return dataTable;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
};

bool UserRepository::UpdatePassword(const int &userEntry, const std::string &newPassword)
{
    try
    {
        const std::string sQuery = "UPDATE Users SET Password = ? WHERE UserEntry = ?";
        const std::vector<std::string> vParams = {
            std::to_string(userEntry),
            newPassword};

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
};
