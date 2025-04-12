#include "User.hpp"

std::vector<User> User::ReadAll() {
    std::vector<User> users;
    DataTable datatable;
    std::string sQuery = "SELECT id, name, email FROM Users";

    try {
        std::shared_ptr<SQL> database = GlobalEnvironment::DatabaseConnection();

        database->PrepareStatement(sQuery);
        datatable.Fill(database->FetchResults(sQuery));

        if(datatable.RowsCount() == 0)
        {
            throw std::runtime_error(std::string("No existen usuarios ")); 
        }

        for (int i = 0; i < datatable.RowsCount(); ++i) {
            User u;
            u.id = std::stoi(datatable[i]["id"]);
            u.name = datatable[i]["name"];
            u.email = datatable[i]["email"];
            users.push_back(u);
        }
    } catch (const std::exception& ex) {
        throw std::runtime_error(std::string("Error al obtener usuarios: ") + ex.what());
    }

    return users;
}

std::optional<User> User::ReadById( int id) {
    std::vector<User> users;
    DataTable oDatatable;
    std::string sQuery = "SELECT id, name, email FROM Users WHERE id = " + std::to_string(id);
    std::shared_ptr<SQL> database = GlobalEnvironment::DatabaseConnection();
    
    try
    {
        std::shared_ptr<SQL> database = GlobalEnvironment::DatabaseConnection();

        oDatatable.Fill(database->FetchResults(sQuery));

        if(oDatatable.RowsCount() == 0)
        {
            throw std::runtime_error(std::string("No existen usuarios ")); 
            return std::nullopt;
        }

        if(oDatatable.RowsCount() > 1)
        {
            throw std::runtime_error(std::string("Existen inconsistencias en la información, hay mas de un usuario con el mismo ID")); 
            return std::nullopt;
        }

        return User {
            std::stoi(oDatatable[0]["ID"]),
            oDatatable[0]["name"],
            oDatatable[0]["email"]
        };    
    }
    catch(const std::exception& ex)
    {
        throw std::runtime_error(std::string("Error al obtener usuario: ") + ex.what());
    }
}

bool User::Create(const std::string& name, const std::string& email) {    
    try
    {
        std::shared_ptr<SQL> database = GlobalEnvironment::DatabaseConnection();
        std::string sQuery = "INSERT INTO Users (name, email) VALUES ('" + name + "', '" + email + "')";

        if(database->RunStatement(sQuery))
        {
            return true;
        }
        else
        {
            throw;
            return false;
        }
    }
    catch(const std::exception& ex)
    {
        throw std::runtime_error(std::string("Exception  creaating a user") + ex.what());
        return false;
    }
}

bool User::Update(int id, const std::string& name, const std::string& email) {
    try
    {
        std::shared_ptr<SQL> database = GlobalEnvironment::DatabaseConnection();
        std::string sQuery = "UPDATE Users SET name = '" + name + "', email = '" + email + "' WHERE id = " + std::to_string(id);
        if(database->RunStatement(sQuery))
        {
            return true;
        }
        else
        {
            throw;
            return false;
        }
    }
    catch(const std::exception& ex)
    {
        throw std::runtime_error(std::string("Exception updating a user") + ex.what());
        return false;
    }
}

bool User::Delete(int id) {
    try
    {
        std::shared_ptr<SQL> database = GlobalEnvironment::DatabaseConnection();
        std::string sQuery =  "DELETE FROM Users WHERE id = " + std::to_string(id);
        if(database->RunStatement(sQuery))
        {
            return true;
        }
        else
        {
            throw;
            return false;
        }
    }
    catch(const std::exception& ex)
    {
        throw std::runtime_error(std::string("Exception deleting a user") + ex.what());
        return false;
    }
}
