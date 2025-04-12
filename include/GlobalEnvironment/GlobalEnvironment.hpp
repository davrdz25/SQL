#pragma one
#include <iostream>
#include "../SQL/SQL.hpp"
#include "../DataTable/DataTable.hpp"

class GlobalEnvironment
{    
    public:
    
        static std::string DBServer;
        static std::string DatabaseName;
        static std::string DBUser;
        static std::string DBPassword;

        static void InitDatabaseConnection(std::shared_ptr<SQL>);
        static std::shared_ptr<SQL> DatabaseConnection();

    private:
        static std::shared_ptr<SQL> Database;
};
