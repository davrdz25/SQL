#include "ItemRepository.hpp"

ItemRepository::ItemRepository(std::shared_ptr<SQL> Database)
    : Database_(std::move(Database)) {}

std::vector<Item> ItemRepository::ReadByCode(const std::string& _ItemCode)
{
    std::vector<Item> items;

    try
    {
        DataTable dataTable;

        std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items WHERE ItemCode = '" + _ItemCode + "'";

        dataTable.Fill(Database_->FetchResults(Query));

        std::cout << "Items desde ReadAll " << dataTable.RowsCount() << std::endl;

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
            std::cout << dataTable[i]["Entry"] << std::endl;
            std::cout << dataTable[i]["ItemName"] << std::endl;
            std::cout << dataTable[i]["ItemCode"] << std::endl;
            std::cout << dataTable[i]["Codebars"] << std::endl;
            std::cout << dataTable[i]["OnHand"] << std::endl;

            items.push_back(Item{
                std::stoi(dataTable[i]["Entry"]),
                dataTable[i]["ItemName"],
                dataTable[i]["ItemCode"],
                dataTable[i]["Codebars"],
                std::stof(dataTable[i]["OnHand"]),
            });
        }

        return items;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return items;
    }
};

std::vector<Item> ItemRepository::ReadByName(const std::string &_Name)
{
    std::vector<Item> items;

    try
    {
        DataTable dataTable;

        std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items WHERE ItemName = '" + _Name + "'";

        dataTable.Fill(Database_->FetchResults(Query));

        std::cout << "Items desde ReadAll " << dataTable.RowsCount() << std::endl;

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
            std::cout << dataTable[i]["Entry"] << std::endl;
            std::cout << dataTable[i]["ItemName"] << std::endl;
            std::cout << dataTable[i]["ItemCode"] << std::endl;
            std::cout << dataTable[i]["Codebars"] << std::endl;
            std::cout << dataTable[i]["OnHand"] << std::endl;

            items.push_back(Item{
                std::stoi(dataTable[i]["Entry"]),
                dataTable[i]["ItemName"],
                dataTable[i]["ItemCode"],
                dataTable[i]["Codebars"],
                std::stof(dataTable[i]["OnHand"]),
            });
        }

        return items;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return items;
    }
};

std::vector<Item> ItemRepository::ReadByCodebars(const std::string& _Codebars) {
    std::vector<Item> items;

    try
    {
        DataTable dataTable;

        std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items WHERE Codebars = '" + _Codebars + "'";

        dataTable.Fill(Database_->FetchResults(Query));

        std::cout << "Items desde ReadAll " << dataTable.RowsCount() << std::endl;

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
            std::cout << dataTable[i]["Entry"] << std::endl;
            std::cout << dataTable[i]["ItemName"] << std::endl;
            std::cout << dataTable[i]["ItemCode"] << std::endl;
            std::cout << dataTable[i]["Codebars"] << std::endl;
            std::cout << dataTable[i]["OnHand"] << std::endl;

            items.push_back(Item{
                std::stoi(dataTable[i]["Entry"]),
                dataTable[i]["ItemName"],
                dataTable[i]["ItemCode"],
                dataTable[i]["Codebars"],
                std::stof(dataTable[i]["OnHand"]),
            });
        }

        return items;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return items;
    }
};

std::vector<Item> ItemRepository::ReadAll()
{
    DataTable dataTable;
    std::vector<Item> items;

    std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items";

    dataTable.Fill(Database_->FetchResults(Query));

    std::cout << "Items desde ReadAll " << dataTable.RowsCount() << std::endl;

    for (int i = 0; i < dataTable.RowsCount(); i++)
    {
        std::cout << dataTable[i]["Entry"] << std::endl;
        std::cout << dataTable[i]["ItemName"] << std::endl;
        std::cout << dataTable[i]["ItemCode"] << std::endl;
        std::cout << dataTable[i]["Codebars"] << std::endl;
        std::cout << dataTable[i]["OnHand"] << std::endl;

        items.push_back(Item{
            std::stoi(dataTable[i]["Entry"]),
            dataTable[i]["ItemName"],
            dataTable[i]["ItemCode"],
            dataTable[i]["Codebars"],
            std::stof(dataTable[i]["OnHand"]),
        });
    }

    return items;
};

std::vector<Item> ItemRepository::ReadByEntry(const int &_Entry)
{
    std::vector<Item> items;

    try
    {
        DataTable dataTable;

        std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items WHERE Entry = " + std::to_string(_Entry);

        dataTable.Fill(Database_->FetchResults(Query));

        std::cout << "Items desde ReadAll " << dataTable.RowsCount() << std::endl;

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
            std::cout << dataTable[i]["Entry"] << std::endl;
            std::cout << dataTable[i]["ItemName"] << std::endl;
            std::cout << dataTable[i]["ItemCode"] << std::endl;
            std::cout << dataTable[i]["Codebars"] << std::endl;
            std::cout << dataTable[i]["OnHand"] << std::endl;

            items.push_back(Item{
                std::stoi(dataTable[i]["Entry"]),
                dataTable[i]["ItemName"],
                dataTable[i]["ItemCode"],
                dataTable[i]["Codebars"],
                std::stof(dataTable[i]["OnHand"]),
            });
        }

        return items;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return items;
    }
};

bool ItemRepository::Create(const Item &_Item)
{
    try
    {
        std::string Query = "INSERT INTO Items VALUES ((SELECT MAX(Entry) + 1 FROM Items), '" + _Item.ItemName + "', " + "'" + _Item.ItemCode + "', '" + _Item.Codebars + "', " + std::to_string(_Item.OnHand) + ")";

        if (Database_->RunStatement(Query))
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
    catch (const std::exception &Ex)
    {
        std::runtime_error(Ex.what());
        return false;
    }
};
bool ItemRepository::Update(const Item &_Item) {

};

bool ItemRepository::Delete(const Item &_Item) {

};