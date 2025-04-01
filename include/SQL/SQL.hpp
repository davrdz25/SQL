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
    
        void extractError(const char *fn, SQLHANDLE handle, SQLSMALLINT type);
    
    public:
        SQL();
        ~SQL();
        void connect(const std::string& dsn, const std::string& dbName, const std::string& user, const std::string& password);
        void prepareStatement(const std::string& query);
        void execute();
        std::vector<std::map<std::string, std::string>> fetchResults();
    };
    