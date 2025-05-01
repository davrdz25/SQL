#include "ItemService.hpp"

ItemService::ItemService(std::shared_ptr<IItemRepository> repository)
    : repository(std::move(repository)) {};

bool ItemService::AddItem(const Item &item)
{
    try
    {
        if (item.Codebars.length() == 0)
            throw std::invalid_argument("Codebars empty");

        if (item.ItemName.length() == 0 || item.ItemName == "")
            throw std::invalid_argument("Name empty");
        
        if (item.ItemCode.length() == 0)
            throw std::invalid_argument("Code empty");

        repository->Create(item);
    }
    catch (const std::exception &Ex)
    {
        std::cerr << "Error ItemService " << Ex.what() << std::endl;
        return false;
    }
}

bool ItemService::ModifyItem(const Item &item)
{
    try
    {
        if(repository->Update(item))
            return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
};

Item ItemService::SearchItem(const int &Entry)
{
    Item item;
    try
    {
        if (Entry == 0 || Entry < 0)
            throw std::invalid_argument("Invalid entry value");
    

        std::optional<Item> itemRepo = repository->ReadByEntry(Entry);

        if (!itemRepo.has_value())
            throw std::invalid_argument("Item not found");

        item = itemRepo.value();

        return item;
    }
    catch (const std::exception &Ex)
    {
        std::cerr << Ex.what() << '\n';
        return item;
    }
}

std::vector<Item> ItemService::SearchItem(Filter filter, const std::string &Value)
{
    std::vector<Item> items;
    try
    {
        std::optional<std::vector<Item>> itemRepo;

        if (Value == "" || Value.size() == 0)
            throw std::invalid_argument("Value canot be empty");

        switch (filter)
        {
        case ItemCode:
            itemRepo = repository->ReadByCode(Value);

            if (!itemRepo.has_value())
                throw std::runtime_error("Items not found");
        break;

        case ItemName:
            itemRepo = repository->ReadByName(Value);

            if (!itemRepo.has_value())
                throw std::runtime_error("Items not found");
        break;

        case Codebars:
            itemRepo = repository->ReadByCodebars(Value);

            if (!itemRepo.has_value())
                throw std::runtime_error("Items not found");
        break;

        default:
            throw std::runtime_error("Items not found");
        }

        items = itemRepo.value();
        return items;
    }
    catch (const std::exception &Ex)
    {
        std::cerr << Ex.what() << '\n';
        return items;
    }
}

std::vector<Item> ItemService::GetAllItems()
{
    std::vector<Item> items;

    try
    {
        std::optional<std::vector<Item>> itemRepo = repository->ReadAll();

        if (!itemRepo.has_value())
            throw std::runtime_error("No items found");

        items = itemRepo.value();

        return items;
    }
    catch (const std::exception &Ex)
    {
        std::cerr << Ex.what() << '\n';
        return items;
    }
}
