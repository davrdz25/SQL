#pragma once
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <stdexcept>
#include <vector>
#include <map>
#include <string>
#include "../Interfaces/ISQL.hpp"

class SQL : public ISQL{
    private:
        SQLHENV henv;
        SQLHDBC hdbc;
        SQLHSTMT hstmt;

        std::string _ServerName;
        std::string _DatabaseName;
        std::string _UserName;
        std::string _Password;
        bool _TrustServerCertificate;

        void PrepareStatement(const std::string& query);
        void ExtractError(const char *fn, SQLHANDLE handle, SQLSMALLINT type);
        void RunQuery(const std::string& query);

    public:
        SQL();
        ~SQL();
        void ServerName(const std::string&);
        void DatabaseName(const std::string&);
        void UserName(const std::string&);
        void Password(const std::string&);
        void TrustServerCertificate(const bool&);
        bool Connect() override;
        bool RunStatement(const std::string& query) override;
        std::vector<std::map<std::string, std::string>> FetchResults(const std::string& query) override;

        bool RunPrepared(const std::string& query, const std::vector<std::string>& params) override;
        std::vector<std::map<std::string, std::string>> FetchPrepared(const std::string& query, const std::vector<std::string>& params) override;
    
    };
    