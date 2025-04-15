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
    
            //Item item{0, "Item desde C++", "1234567890","ItmC++", 99.9f};
            //service.CreateItem(item);

            std::vector<Item> items = service.SearchItem(1);
            std::vector<Item> items2 = service.SearchItem(ItemCode, "LAP123");

            std::cout << "Items desde main " << std::endl;

            for(int i = 0; i < items.size(); i++)
            {
                std::cout << items[i].Entry << std::endl;
                std::cout << items[i].ItemName << std::endl;
                std::cout << items[i].ItemCode << std::endl;
                std::cout << items[i].Codebars << std::endl;
                std::cout << items[i].OnHand << std::endl;
            }

            std::cout << "--------Items desde filtrado por ItemCode------" << std::endl;

            for(int i = 0; i < items2.size(); i++)
            {
                std::cout << items2[i].Entry << std::endl;
                std::cout << items2[i].ItemName << std::endl;
                std::cout << items2[i].ItemCode << std::endl;
                std::cout << items2[i].Codebars << std::endl;
                std::cout << items2[i].OnHand << std::endl;
            }
            
        }
        else {
            std::cout << "No conectado" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
