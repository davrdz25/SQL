#include "ItemService.hpp"

ItemService::ItemService(std::shared_ptr<IItemRepository> repository)
        : repository(std::move(repository)) {};

bool ItemService::AddItem(const Item& item){
    try
    {
        if(item.Codebars.length() == 0){
            throw std::invalid_argument("Codebars must be greater than 0");
            return false;
        }

        if(item.ItemName.length() == 0){
            throw std::invalid_argument("ItemName must be greater than 0");
            return false;
        }

        if(item.ItemCode.length() == 0){
            throw std::invalid_argument("ItemCode must be greater than 0");
            return false;
        }

        repository->Create(item);

    }
    catch(const std::exception& Ex)
    {
        std::runtime_error(Ex.what());
        return false;
    }
}
Item ItemService::SearchItem(const int& Entry)
{
    Item item;

    try
    {
        if(Entry == 0)
        {
            throw std::invalid_argument("Entry must be greater than 0");
            return item;
        }

        if(Entry < 0)
        {
            throw std::invalid_argument("Entry must be greater positive");
            return item;
        }

        if(repository)
        item = repository->ReadByEntry(Entry);

        return item;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return item;
    }
    
}

std::vector<Item> ItemService::SearchItem(Filter filter, const std::string& Value)
{
    std::vector<Item> items;

    try
    {
        if(Value == "" || Value.size() == 0)
        {
            throw std::invalid_argument("Value canot be empty");
            return items;
        }

        switch (filter)
        {
        case ItemCode:
            items = repository->ReadByCode(Value);
            break;

            case ItemName:
            items = repository->ReadByName(Value);
            break;

            case Codebars:
            items = repository->ReadByCodebars(Value);
            break;
        
        default:
            break;
        }

        if(items.size() == 0)
        {
            throw std::runtime_error("No items found");
        }

        return items;

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return items;
    }
    
}

std::vector<Item> ItemService::GetAllItems()
{
    std::vector<Item> items;

    try
    {
        if(repository->ReadAll().size() == 0)
        {
            throw std::runtime_error("No items found");
            return items;
        }
        else
        {
            items = repository->ReadAll();
        }
        
        return items;

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return items;
    }
}

bool ItemService::ModifyItem(const Item& item)
{
    try
    {

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
};
