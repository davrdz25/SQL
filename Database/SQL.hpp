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

        void PrepareStatement(const std::string&);
        void ExtractError(const char *, SQLHANDLE, SQLSMALLINT);
        void RunQuery(const std::string&);

    public:
        SQL();
        ~SQL();
        void ServerName(const std::string&);
        void DatabaseName(const std::string&);
        void UserName(const std::string&);
        void Password(const std::string&);
        void TrustServerCertificate(const bool&);
        bool Connect() override;
        bool RunStatement(const std::string&) override;
        std::vector<std::map<std::string, std::string>> FetchResults(const std::string&) override;

        bool RunPrepared(const std::string&, const std::vector<std::string>&) override;
        bool RunPrepared(const std::string &,const std::vector<std::string> &, const std::vector<std::vector<uint8_t>> &) override;
        std::vector<std::map<std::string, std::string>> FetchPrepared(const std::string&, const std::vector<std::string>&) override;
        std::vector<std::map<std::string, std::string>> FetchPrepared(const std::string&, const std::string&) override;
    };
    