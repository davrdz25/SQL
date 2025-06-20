#include "SQL.hpp"

std::string SQL::ExtractError(const char *fn, SQLHANDLE handle, SQLSMALLINT type)
{
    if (!handle)
    {
        return std::string(" [ExtractError] Handle nulo o no inicializado.");
    }

    SQLINTEGER i = 0;
    SQLINTEGER native;
    SQLCHAR state[7];
    SQLCHAR text[256];
    SQLSMALLINT len;
    SQLRETURN ret;

    std::string errors;
    do
    {
        ret = SQLGetDiagRec(type, handle, ++i, state, &native, text, sizeof(text), &len);
        if (SQL_SUCCEEDED(ret))
        {
            errors += "SQL State: ";
            errors += reinterpret_cast<const char *>(state);
            errors += "\nMessage: ";
            errors += std::string(reinterpret_cast<const char *>(text), len);
        }
        else if (ret == SQL_INVALID_HANDLE)
        {
            errors += "[ExtractError] SQL_INVALID_HANDLE";
            break;
        }
    } while (ret == SQL_SUCCESS);

    return errors;
}

SQL::SQL() : henv(SQL_NULL_HENV), hdbc(SQL_NULL_HDBC), hstmt(SQL_NULL_HSTMT) {}

SQL::~SQL()
{
    Disconnect();
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

bool SQL::Connect()
{
    try
    {
        SQLRETURN ret;

        std::string connString = "Driver={ODBC Driver 18 for SQL Server};Server=" + _ServerName + ";UID=" + _UserName + ";PWD=" + _Password + ";Database=" + _DatabaseName + ";TrustServerCertificate=yes;";

        ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error(ExtractError("SQLAllocHandle ENV", henv, SQL_HANDLE_ENV));

        ret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error(ExtractError("SQLSetEnvAttr", henv, SQL_HANDLE_ENV));

        ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error(ExtractError("SQLAllocHandle DBC", henv, SQL_HANDLE_ENV));

        ret = SQLDriverConnect(
            hdbc, nullptr,
            (SQLCHAR *)connString.c_str(),
            SQL_NTS, nullptr, 0, nullptr, SQL_DRIVER_NOPROMPT);

        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error(ExtractError("SQLConnect", hdbc, SQL_HANDLE_DBC));

        return true;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("[SQL::Connect] ") + e.what());
    }
}

void SQL::Disconnect()
{
    try
    {
        if (hdbc != SQL_NULL_HDBC)
        {
            SQLDisconnect(hdbc);
            SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
            hdbc = SQL_NULL_HDBC;
        }
        if (henv != SQL_NULL_HENV)
        {
            SQLFreeHandle(SQL_HANDLE_ENV, henv);
            henv = SQL_NULL_HENV;
        }
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("[SQL::Disconnect] ") + e.what() + "\n" +
                                 ExtractError("SQL::Disconnect", hdbc, SQL_HANDLE_DBC));
    }
}

void SQL::PrepareStatement(const std::string &query)
{
    try
    {
        if (hstmt != SQL_NULL_HSTMT)
        {
            SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
            hstmt = SQL_NULL_HSTMT;
        }

        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error("No se pudo asignar el handle para el statement. " + ExtractError("SQLAllocHandle", hdbc, SQL_HANDLE_DBC));

        ret = SQLPrepare(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);

        std::cout << "Query: " << query << std::endl;

        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error("Error al preparar la consulta: " + ExtractError("SQLPrepare", hstmt, SQL_HANDLE_STMT));
    }
    catch (const std::exception &ex)
    {
        throw std::runtime_error(std::string("[SQL::PrepareStatement] ") + ex.what());
    }
}

bool SQL::RunStatement(const std::string &query)
{
    try
    {
        SQLRETURN ret;

        if (hstmt != SQL_NULL_HSTMT)
        {
            SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
            hstmt = SQL_NULL_HSTMT;
        }

        ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error("SQLAllocHandle STMT failed");

        ret = SQLExecDirect(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);

        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error("SQLExecDirect failed");

        return true;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("[SQL::RunStatement] ") + e.what() + " " + ExtractError("SQL::RunStatement", hstmt, SQL_HANDLE_STMT));
    }
}

bool SQL::BeginTransaction()
{
    try
    {
        SQLRETURN ret = SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 0);
        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error("Failed to disable autocommit");
        return true;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("[SQL::BeginTransaction] ") + e.what() + "\n" +
                                 ExtractError("SQL::BeginTransaction", hdbc, SQL_HANDLE_DBC));
    }
}

bool SQL::CommitTransaction()
{
    try
    {
        SQLRETURN ret = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);

        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error("Failed to commit transaction");

        ret = SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0);

        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error("Failed to enable autocommit after commit");

        return true;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("[SQL::Commit] ") + e.what() + "\n" +
                                 ExtractError("SQL::Commit", hdbc, SQL_HANDLE_DBC));
    }
}

bool SQL::RollbackTransaction()
{
    try
    {
        SQLRETURN ret = SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);

        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error("Failed to rollback transaction");

        ret = SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0);

        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error("Failed to enable autocommit after rollback");

        return true;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("[SQL::Rollback] ") + e.what() + "\n" +
                                 ExtractError("SQL::Rollback", hdbc, SQL_HANDLE_DBC));
    }
}

bool SQL::RunPrepared(const std::string &query, const std::vector<std::string> &params)
{
    try
    {
        PrepareStatement(query);

        for (size_t i = 0; i < params.size(); ++i)
        {
            for (size_t i = 0; i < params.size(); ++i)
            {
                SQLLEN indPtr;
                const char *paramValue = nullptr;

                if (params[i].empty())
                {
                    indPtr = SQL_NULL_DATA;
                }
                else
                {
                    paramValue = params[i].c_str();
                    indPtr = SQL_NTS;
                }

                SQLRETURN retcode = SQLBindParameter(
                    hstmt,
                    static_cast<SQLUSMALLINT>(i + 1),
                    SQL_PARAM_INPUT,
                    SQL_C_CHAR,
                    SQL_VARCHAR,
                    0, 0,
                    (SQLPOINTER)paramValue,
                    0,
                    &indPtr);

                if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
                    throw std::runtime_error("Error al enlazar parámetro " + std::to_string(i + 1));
            }

            SQLRETURN retcode = SQLBindParameter(
                hstmt,
                (SQLUSMALLINT)(i + 1),
                SQL_PARAM_INPUT,
                SQL_C_CHAR,
                SQL_VARCHAR,
                0, 0,
                (SQLPOINTER)params[i].c_str(),
                0,
                NULL);

            if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
                throw std::runtime_error("Error linkin parameter " + std::to_string(i + 1));
        }

        SQLRETURN retcode = SQLExecute(hstmt);

        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
            throw std::runtime_error(ExtractError("SQLExecute", hstmt, SQL_HANDLE_STMT));

        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        hstmt = nullptr;
        return true;
    }
    catch (const std::exception &ex)
    {

        std::cerr << "[RunPrepared] Excepción: " << ex.what() << std::endl;
        if (hstmt)
        {
            SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
            hstmt = nullptr;
        }
        return false;
    }
}

bool SQL::RunPrepared(const std::string &query, const std::vector<std::optional<std::string>> &params, const std::vector<std::vector<uint8_t>> &binaryParams)
{
    try
    {
        SQLRETURN retcode;
        SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

        PrepareStatement(query);

        size_t paramIndex = 1;
        size_t strIndex = 0;
        size_t binIndex = 0;

        // Enlazar parámetros de texto
        for (; strIndex < params.size(); ++strIndex, ++paramIndex)
        {
            char dummy = 0;

            SQLLEN indPtr;
            const char *valuePtr = nullptr;

            if (params[strIndex].has_value())
            {
                valuePtr = params[strIndex]->c_str();
                indPtr = SQL_NTS;
            }
            else
            {
                valuePtr = &dummy; 
                indPtr = SQL_NULL_DATA;
            }

            retcode = SQLBindParameter(
                hstmt,
                paramIndex,
                SQL_PARAM_INPUT,
                SQL_C_CHAR,
                SQL_VARCHAR,
                0,
                0,
                (SQLPOINTER)valuePtr,
                0,
                &indPtr);

            if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
                throw std::runtime_error("Error al enlazar parámetro de texto " + std::to_string(paramIndex));
        }

        // Enlazar parámetros binarios
        for (; binIndex < binaryParams.size(); ++binIndex, ++paramIndex)
        {
            SQLLEN dataSize = static_cast<SQLLEN>(binaryParams[binIndex].size());

            retcode = SQLBindParameter(
                hstmt,
                paramIndex,
                SQL_PARAM_INPUT,
                SQL_C_BINARY,
                SQL_VARBINARY,
                dataSize, 0,
                (SQLPOINTER)binaryParams[binIndex].data(),
                dataSize,
                &dataSize); // muy importante: pasar dirección del tamaño real

            if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
                throw std::runtime_error("Error al enlazar parámetro binario " + std::to_string(paramIndex));
        }

        retcode = SQLExecute(hstmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
            throw std::runtime_error("Error al ejecutar la consulta preparada. " + ExtractError("SQLExecute", hstmt, SQL_HANDLE_STMT));

        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        return true;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "[CreateUser Exception]  [RunPrepared binario] Excepción: " << ex.what() << std::endl;
        throw std::runtime_error(ex.what());
        return false;
    }
}

DataTable SQL::FetchResults(const std::string &query)
{
    try
    {
        DataTable dataTable;

        if (hstmt != SQL_NULL_HSTMT)
        {
            SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
            hstmt = SQL_NULL_HSTMT;
        }

        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

        if (!SQL_SUCCEEDED(ret))
            throw std::runtime_error("No se pudo asignar el handle para el statement. " + ExtractError("SQLAllocHandle", hdbc, SQL_HANDLE_DBC));

        SQLRETURN retcode = SQLExecDirect(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);

        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
            throw std::runtime_error("Error al ejecutar la consulta. " + ExtractError("SQLExecDirect", hstmt, SQL_HANDLE_STMT));

        SQLSMALLINT columnCount;
        SQLNumResultCols(hstmt, &columnCount);

        SQLCHAR columnName[256];
        SQLSMALLINT columnNameLength, dataType, decimalDigits, nullable;
        SQLULEN columnSize;

        std::vector<std::string> columnNames;

        for (SQLUSMALLINT i = 1; i <= columnCount; ++i)
        {
            SQLDescribeCol(hstmt, i, columnName, sizeof(columnName), &columnNameLength,
                           &dataType, &columnSize, &decimalDigits, &nullable);

            columnNames.emplace_back(reinterpret_cast<char *>(columnName), columnNameLength);
        }

        std::vector<DataTable::Row> rows;

        while (SQLFetch(hstmt) == SQL_SUCCESS)
        {
            DataTable::Row row;
            row.SetColumns(columnNames);

            for (SQLUSMALLINT i = 1; i <= columnCount; ++i)
            {
                SQLCHAR buffer[1024];
                SQLLEN indicator;

                retcode = SQLGetData(hstmt, i, SQL_C_CHAR, buffer, sizeof(buffer), &indicator);

                if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
                {
                    if (indicator == SQL_NULL_DATA)
                        row[columnNames[i - 1]] = std::nullopt;
                    else
                        row[columnNames[i - 1]] = std::string(reinterpret_cast<char *>(buffer));
                }
                else
                {
                    throw std::runtime_error("Error al obtener los datos de la columna " + columnNames[i - 1]);
                }
            }

            rows.push_back(row);
        }

        dataTable.Fill(rows);

        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        hstmt = SQL_NULL_HSTMT;

        return dataTable;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "[FetchResults] Excepción: " << ex.what() << std::endl;
        throw;
    }
}

DataTable SQL::FetchPrepared(const std::string &query, const std::vector<std::string> &params)
{
    DataTable dataTable;

    try
    {
        PrepareStatement(query);

        for (size_t i = 0; i < params.size(); ++i)
        {
            SQLRETURN retcode = SQLBindParameter(
                hstmt,
                (SQLUSMALLINT)(i + 1),
                SQL_PARAM_INPUT,
                SQL_C_CHAR,
                SQL_VARCHAR,
                0, 0,
                (SQLPOINTER)params[i].c_str(),
                0,
                NULL);

            if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
                throw std::runtime_error("Error al enlazar parámetro " + std::to_string(i + 1));
        }

        SQLRETURN retcode = SQLExecute(hstmt);

        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
            throw std::runtime_error("Error al ejecutar la consulta preparada. " + ExtractError("SQLExecute", hstmt, SQL_HANDLE_STMT));

        SQLSMALLINT columnCount;
        SQLNumResultCols(hstmt, &columnCount);

        std::vector<std::string> columnNames(columnCount);
        SQLCHAR columnName[256];
        SQLSMALLINT columnNameLength, nativeType, decimalDigits, nullable;
        SQLULEN columnSize;

        for (SQLUSMALLINT i = 1; i <= columnCount; ++i)
        {
            SQLDescribeCol(hstmt, i, columnName, sizeof(columnName), &columnNameLength,
                           &nativeType, &columnSize, &decimalDigits, &nullable);
            columnNames[i - 1] = std::string(reinterpret_cast<char *>(columnName), columnNameLength);
        }

        std::vector<DataTable::Row> rows;

        while (SQLFetch(hstmt) == SQL_SUCCESS)
        {
            DataTable::Row row;
            row.SetColumns(columnNames);

            for (SQLUSMALLINT i = 1; i <= columnCount; ++i)
            {
                SQLCHAR value[256];
                SQLLEN indicator;
                retcode = SQLGetData(hstmt, i, SQL_C_CHAR, value, sizeof(value), &indicator);

                if (indicator == SQL_NULL_DATA)
                    row[columnNames[i - 1]] = std::nullopt;
                else
                    row[columnNames[i - 1]] = std::string(reinterpret_cast<char *>(value));
            }

            rows.push_back(std::move(row));
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        hstmt = nullptr;

        dataTable.Fill(rows);
    }
    catch (const std::exception &ex)
    {
        std::cerr << "[FetchPrepared] Excepción: " << ex.what() << std::endl;
    }

    return dataTable;
}

DataTable SQL::FetchPrepared(const std::string &query, const std::string &param)
{
    try
    {
        return FetchPrepared(query, std::vector<std::string>{param});
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
