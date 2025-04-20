#include "ItemService.hpp"

ItemService::ItemService(std::shared_ptr<IItemRepository> repository)
        : repository(std::move(repository)) {};

bool ItemService::AddItem(const Item& item)
{
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

std::optional<Item> ItemService::SearchItem(const int& Entry)
{

    try
    {
        if(Entry == 0)
        {
            throw std::invalid_argument("Entry must be greater than 0");
            return std::nullopt;
        }

        if(Entry < 0)
        {
            throw std::invalid_argument("Entry must be greater positive");
            return std::nullopt;
        }

        if(repository->ReadByEntry(Entry).has_value())
        {
            Item item;

            item = repository->ReadByEntry(Entry).value();

            return item;
        }

        throw std::invalid_argument("Item not found or foudn more than one");
        return std::nullopt;
            
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
    
}

std::optional<std::vector<Item>> ItemService::SearchItem(Filter filter, const std::string& Value)
{

    try
    {
        std::vector<Item> items;

        if(Value == "" || Value.size() == 0)
        {
            throw std::invalid_argument("Value canot be empty");
            return std::nullopt;
        }

        switch (filter)
        {
        case ItemCode:
            items = repository->ReadByCode(Value).value();
            return items;
            break;

        case ItemName:
            items = repository->ReadByName(Value).value();
            return items;
            break;

        case Codebars:
            items = repository->ReadByCodebars(Value).value();
            return items;
            break;
        
        default:
            return std::nullopt;
            break;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
    
}

std::optional<std::vector<Item>> ItemService::GetAllItems()
{

    try
    {
        std::vector<Item> items;

        if(!repository->ReadAll().has_value())
        {
            throw std::runtime_error("No items found");
            return std::nullopt;
        }
        
        items = repository->ReadAll().value();
        
        return items;

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;
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
