#include "SQL.hpp"

void SQL::ExtractError(const char *fn, SQLHANDLE handle, SQLSMALLINT type)
{
    SQLINTEGER i = 0;
    SQLINTEGER native;
    SQLCHAR state[7];
    SQLCHAR text[256];
    SQLSMALLINT len;
    SQLRETURN ret;

    std::cerr << "Error en " << fn << ":" << std::endl;
    do
    {
        ret = SQLGetDiagRec(type, handle, ++i, state, &native, text, sizeof(text), &len);
        if (SQL_SUCCEEDED(ret))
        {
            std::cerr << text << std::endl;
        }
    } while (ret == SQL_SUCCESS);
}

SQL::SQL()
{
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
}

SQL::~SQL()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    SQLDisconnect(hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

bool SQL::Connect()
{
    try
    {
        std::string connString = "Driver={ODBC Driver 18 for SQL Server};Server=" + _ServerName + ";Database=" + _DatabaseName + ";UID=" + _UserName + ";PWD=" + _Password + ";TrustServerCertificate=" + (_TrustServerCertificate ? "yes" : "no") + ";APP=DebugC++";
        SQLRETURN retcode = SQLDriverConnect(hdbc, NULL, (SQLCHAR *)connString.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            ExtractError("SQLConnect", hdbc, SQL_HANDLE_DBC);
            return false;
        }
        else
        {
            return true;
        }
    }
    catch (const std::exception &ex)
    {
        throw std::runtime_error(std::string("Error conectando a la base de datos: ") + ex.what());
    }
}

void SQL::PrepareStatement(const std::string &query)
{
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    SQLPrepare(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);
}

void SQL::Execute()
{
    SQLExecute(hstmt);
}

std::vector<std::map<std::string, std::string>> SQL::FetchResults(const std::string &query)
{
    std::vector<std::map<std::string, std::string>> results;

    try
    {
        SQLCHAR columnName[256];
        SQLSMALLINT columnCount;
        SQLSMALLINT columnNameLength, nativeType, decimalDigits, nullable;
        SQLULEN columnSize;
        SQLRETURN retcode;

        SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
        SQLPrepare(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);

        retcode = SQLExecDirect(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            ExtractError("RunStatement", hstmt, SQL_HANDLE_STMT);
            throw std::runtime_error("Error al ejecutar la consulta: " + query);
        }

        SQLNumResultCols(hstmt, &columnCount);

        std::vector<std::string> columnNames;
        for (SQLUSMALLINT i = 1; i <= columnCount; ++i)
        {
            SQLDescribeCol(hstmt, i, columnName, sizeof(columnName), &columnNameLength, &nativeType, &columnSize, &decimalDigits, &nullable);
            columnNames.push_back(std::string(reinterpret_cast<char *>(columnName)));
        }

        while (SQLFetch(hstmt) == SQL_SUCCESS)
        {
            std::map<std::string, std::string> row;
            for (SQLUSMALLINT i = 1; i <= columnCount; ++i)
            {
                SQLCHAR value[256];
                SQLLEN indicator;
                retcode = SQLGetData(hstmt, i, SQL_C_CHAR, value, sizeof(value), &indicator);
                row[columnNames[i - 1]] = (indicator != SQL_NULL_DATA) ? std::string(reinterpret_cast<char *>(value)) : "NULL";
            }
            results.push_back(row);
        }
        std::cout << "Tamanho desde consulta SQL " + std::to_string(results.size()) << std::endl;
        return results;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return results;
    }
}

bool SQL::RunStatement(const std::string &query)
{
    try
    {
        SQLRETURN retcode;
        SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
        retcode = SQLExecDirect(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);

        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            ExtractError("RunStatement", hstmt, SQL_HANDLE_STMT);
            throw std::runtime_error("Error al ejecutar la consulta: " + query);
            return false;
        }
        else
        {
            return true;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Excepción en RunStatement: " << ex.what() << std::endl;
        throw;
        return false;
    }
}

void SQL::ServerName(const std::string &_ServerName)
{
    this->_ServerName = _ServerName;
};
void SQL::DatabaseName(const std::string &_DatabaseName)
{
    this->_DatabaseName = _DatabaseName;
};
void SQL::UserName(const std::string &_UserName)
{
    this->_UserName = _UserName;
};
void SQL::Password(const std::string &_Password)
{
    this->_Password = _Password;
};
void SQL::TrustServerCertificate(const bool &_TrustServerCertificate)
{
    this->_TrustServerCertificate = _TrustServerCertificate;
};