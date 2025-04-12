#include "GlobalEnvironment.hpp"

std::string GlobalEnvironment::DBServer = "";
std::string GlobalEnvironment::DatabaseName = "";
std::string GlobalEnvironment::DBUser = "";
std::string GlobalEnvironment::DBPassword = "";

std::shared_ptr<SQL> GlobalEnvironment::Database = nullptr;

void GlobalEnvironment::InitDatabaseConnection(std::shared_ptr<SQL> SQLConnection) {
    Database = SQLConnection;
}

std::shared_ptr<SQL> GlobalEnvironment::DatabaseConnection() {
    return Database;
}