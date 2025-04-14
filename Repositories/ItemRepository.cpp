#include "ItemRepository.hpp"

ItemRepository::ItemRepository(std::shared_ptr<SQL> Database)
    : Database_(std::move(Database)) {}

std::optional<Item> ItemRepository::ReadByName(const std::string& _Name)
{
};

std::optional<Item> ItemRepository::ReadByCodebars(const std::string& _Codebars)
{

};

std::optional<Item> ItemRepository::ReadAll()
{

};


std::optional<Item> ItemRepository::ReadByEntry(const int& _Entry)
{

};

bool ItemRepository::Create(const Item& _Item)
{
    try
    {
        std::string Query = "INSERT INTO Items VALUES ((SELECT MAX(Entry) + 1 FROM Items), '" + _Item.ItemName + "', " + "'" + _Item.ItemCode + "', '" + _Item.Codebars + "', " + std::to_string(_Item.OnHand) + ")";

        if(Database_->RunStatement(Query))
        {
            std::cout << "Item created" << std::endl;
            return true;
        }
        else
        {
            throw std::runtime_error("Error creating item");
            return false;
        }
    }
    catch(const std::exception& Ex)
    {
        std::runtime_error(Ex.what());
        return false;
    }
};
bool ItemRepository::Update(const Item& _Item)
{

};

bool ItemRepository::Delete(const Item& _Item)
{

};