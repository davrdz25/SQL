#include "Repositories/ItemRepository.hpp"
#include "Services/ItemService.hpp"

int main() {
    try {
        std::shared_ptr Database = std::make_shared<SQL>();

        Database->ServerName("192.168.1.253");
        Database->UserName("sa");
        Database->Password("Development..");
        Database->DatabaseName("POS");
        Database->TrustServerCertificate(true);

        if(Database->Connect())
        {
            std::cout << "Conectado" << std::endl;          

        }
        else {
            std::cout << "No conectado" << std::endl;
        }

        std::shared_ptr itemRepository = std::make_shared<ItemRepository>(Database);
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
