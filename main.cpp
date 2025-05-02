#include "Repositories/ItemRepository.hpp"
#include "Services/ItemService.hpp"

int main()
{
    try
    {
        std::shared_ptr Database = std::make_shared<SQL>();

        Database->ServerName("192.168.1.253");
        Database->UserName("sa");
        Database->Password("B1Admin*");
        Database->DatabaseName("SBODEMOMX");
        Database->TrustServerCertificate(true);

        if (Database->Connect())
        {
            std::cout << "Conectado" << std::endl;

            std::shared_ptr itemRepository = std::make_shared<ItemRepository>(Database);
            ItemService service(itemRepository);

            std::vector<Item> items = service.GetAllItems();

            for (int i = 0; i < items.size(); i++)
            {
                std::cout << items[i].Entry << std::endl;
                std::cout << items[i].ItemCode << std::endl;
                std::cout << items[i].ItemName << std::endl;
                std::cout << items[i].Codebars << std::endl;
                std::cout << items[i].OnHand << std::endl;
            }
            /*std::cout << "Items desde main " << std::endl;

            Item item = service.SearchItem(1).value();
            std::cout << "Items desde main 1" << std::endl;

            std::vector<Item> items2 = service.SearchItem(ItemCode, "LAP123").value();

            std::cout << "Items desde main 2" << std::endl; */

            /* std::cout << "Items desde main " << std::endl;

            std::cout << item.Entry << std::endl;
            std::cout << item.ItemName << std::endl;
            std::cout << item.ItemCode << std::endl;
            std::cout << item.Codebars << std::endl;
            std::cout << item.OnHand << std::endl;

            item.Codebars = "test";

            std::cout << "Actualizando Items desde main " << std::endl;

            std::cout << item.Entry << std::endl;
            std::cout << item.ItemName << std::endl;
            std::cout << item.ItemCode << std::endl;
            std::cout << item.Codebars << std::endl;
            std::cout << item.OnHand << std::endl;

            itemRepository->Update(item);

            std::cout << "Item actualizado desde main " << std::endl;

            Item item2 = service.SearchItem(1).value();


            std::cout << item2.Entry << std::endl;
            std::cout << item2.ItemName << std::endl;
            std::cout << item2.ItemCode << std::endl;
            std::cout << item2.Codebars << std::endl;
            std::cout << item2.OnHand << std::endl;


            std::cout << "--------Items desde filtrado por ItemCode------" << std::endl;

            for (int i = 0; i < items2.size(); i++)
            {
                std::cout << items2[i].Entry << std::endl;
                std::cout << items2[i].ItemName << std::endl;
                std::cout << items2[i].ItemCode << std::endl;
                std::cout << items2[i].Codebars << std::endl;
                std::cout << items2[i].OnHand << std::endl;
            } */
        }
        else
        {
            std::cout << "No conectado" << std::endl;
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error desde main: " << e.what() << std::endl;
        return -1;
    }
}
