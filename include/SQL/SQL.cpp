#include "SQL.hpp"

void SQL::extractError(const char *fn, SQLHANDLE handle, SQLSMALLINT type)
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
            std::cerr << text << std::endl;
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

void SQL::connect(const std::string &dsn, const std::string &dbName, const std::string &user, const std::string &password)
{
    std::string connString = "Driver={ODBC Driver 18 for SQL Server};Server=" + dsn + ";Database=" + dbName + ";UID=" + user + ";PWD=" + password + ";TrustServerCertificate=yes;APP=DebugC++";
    SQLRETURN retcode = SQLDriverConnect(hdbc, NULL, (SQLCHAR *)connString.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
    {
        extractError("SQLConnect", hdbc, SQL_HANDLE_DBC);
        throw std::runtime_error("Error conectando a la base de datos");
    }
}

void SQL::prepareStatement(const std::string &query)
{
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    SQLPrepare(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);
}

void SQL::execute()
{
    SQLExecute(hstmt);
}

std::vector<std::map<std::string, std::string>> SQL::fetchResults()
{
    std::vector<std::map<std::string, std::string>> results;
    SQLCHAR columnName[256];
    SQLSMALLINT columnCount;
    SQLSMALLINT columnNameLength, nativeType, decimalDigits, nullable;
    SQLULEN columnSize;
    SQLRETURN retcode;

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

    if (results.empty())
    {
        throw std::runtime_error("No hay resultados en la consulta");
    }

    return results;
}
