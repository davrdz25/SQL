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
            
            std::shared_ptr itemRepository = std::make_shared<ItemRepository>(Database);
            ItemService service(itemRepository);
    
            Item item{0, "Item desde C++", "1234567890","ItmC++", 99.9f};
            service.CreateItem(item);
        }
        else {
            std::cout << "No conectado" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
