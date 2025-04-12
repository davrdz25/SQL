#include "include/GlobalEnvironment/GlobalEnvironment.hpp"

int main() {
    try {
        GlobalEnvironment::DBServer = "192.168.1.253";
        GlobalEnvironment::DatabaseName = "POS";
        GlobalEnvironment::DBUser = "sa";
        GlobalEnvironment::DBPassword = "Development..";

        std::shared_ptr<SQL> Database = std::make_shared<SQL>();

        GlobalEnvironment::InitDatabaseConnection(Database);
                
        Database->Connect(GlobalEnvironment::DBServer, GlobalEnvironment::DatabaseName, GlobalEnvironment::DBUser,GlobalEnvironment::DBPassword);
        /* DataTable DataTable;
        DataTable.Fill(controller.FetchResults("SELECT ItemName, ItemCode FROM Items"));
        std::cout << DataTable.RowsCount() << std::endl;

        for (int i = 0; i < DataTable.RowsCount(); ++i) {
            std::cout << "Row " << i << ":" << std::endl;
            std::cout << "ItemName: " << DataTable[i]["ItemName"] << std::endl;
            std::cout << "ItemCode: " << DataTable[i]["ItemCosdde"] << std::endl;
        } */
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
