#pragma one
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <stdexcept>
#include <vector>
#include <map>
#include <string>


class SQL {
    private:
        SQLHENV henv;
        SQLHDBC hdbc;
        SQLHSTMT hstmt;
       

    public:
        SQL();
        ~SQL();
        void Execute();
        void CloseConnection();
        void ExtractError(const char *fn, SQLHANDLE handle, SQLSMALLINT type);
        void PrepareStatement(const std::string& query);
        void RunQuery(const std::string& query);
        void Connect(const std::string& dsn, const std::string& dbName, const std::string& user, const std::string& password);
        bool RunStatement(const std::string& query);
        std::vector<std::map<std::string, std::string>> FetchResults(const std::string& query);
    };
    