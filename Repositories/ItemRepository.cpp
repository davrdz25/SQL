#include "ItemRepository.hpp"

ItemRepository::ItemRepository(std::shared_ptr<SQL> Database): Database(std::move(Database)) 
{}

std::optional<std::vector<Item>> ItemRepository::ReadByCode(const std::string &_ItemCode)
{
    try
    {
        DataTable dataTable;

        std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items WHERE ItemCode = '" + _ItemCode + "'";

        dataTable.Fill(Database->FetchResults(Query));

        if (dataTable.RowsCount() > 0)
        {
            std::vector<Item> items;

            for (int i = 0; i < dataTable.RowsCount(); i++)
            {
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

        return std::nullopt;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
};

std::optional<std::vector<Item>> ItemRepository::ReadByName(const std::string &_Name)
{
    try
    {
        std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items WHERE ItemName = '" + _Name + "'";

        DataTable dataTable;
        
        dataTable.Fill(Database->FetchResults(Query));

        if(dataTable.RowsCount() == 0)
            return std::nullopt;
            
        std::vector<Item> items;

        std::cout << "Results length: " << std::to_string(dataTable.RowsCount()) << std::endl;

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
            items.push_back(Item{
                std::stoi(dataTable[i]["Entry"]),
                dataTable[i]["ItemName"],
                dataTable[i]["ItemCode"],
                dataTable[i]["Codebars"],
                std::stof(dataTable[i]["OnHand"]),
            });
        }

        std::cout << "Items by name length: " << std::to_string(items.size()) << std::endl;
        return items;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
};

std::optional<std::vector<Item>> ItemRepository::ReadByCodebars(const std::string &_Codebars)
{
    try
    {
        DataTable dataTable;

        std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items WHERE Codebars = '" + _Codebars + "'";

        dataTable.Fill(Database->FetchResults(Query));

        if (dataTable.RowsCount() == 0)
            return std::nullopt;

        std::vector<Item> items;

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
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
        return std::nullopt;
    }
};

std::optional<std::vector<Item>> ItemRepository::ReadAll()
{
    try
    {
        DataTable dataTable;
        std::vector<Item> items;

        std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items";

        dataTable.Fill(Database->FetchResults(Query));

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
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
        return std::nullopt;
    }
};

std::optional<Item> ItemRepository::ReadByEntry(const int &_Entry)
{
    try
    {
        DataTable dataTable;
        Item item;

        std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items WHERE Entry = " + std::to_string(_Entry);

        dataTable.Fill(Database->FetchResults(Query));


        if (dataTable.RowsCount() == 1)
        {
            item.Entry = std::stoi(dataTable[0]["Entry"]);
            item.ItemName = dataTable[0]["ItemName"];
            item.ItemCode = dataTable[0]["ItemCode"];
            item.Codebars = dataTable[0]["Codebars"];
            item.OnHand = std::stof(dataTable[0]["OnHand"]);

            return item;
        }

        return std::nullopt;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
};

bool ItemRepository::Create(const Item &_Item)
{
    try
    {
        std::string Query = "INSERT INTO Items VALUES ((SELECT MAX(Entry) + 1 FROM Items), '" + _Item.ItemName + "', " + "'" + _Item.ItemCode + "', '" + _Item.Codebars + "', " + std::to_string(_Item.OnHand) + ")";

        if (Database->RunStatement(Query))

            return true;

        return false;
    }
    catch (const std::exception &Ex)
    {
        std::runtime_error(Ex.what());
        return false;
    }
};

bool ItemRepository::Update(const Item &_Item)
{
    try
    {
        std::string Query = "UPDATE Items SET ItemName = '" + _Item.ItemName + "', ItemCode = '" + _Item.ItemCode + "', Codebars = '" + _Item.Codebars + "' , OnHand = " + std::to_string(_Item.OnHand) + " WHERE Entry = " + std::to_string(_Item.Entry);

        if (Database->RunStatement(Query))
            return true;

        return false;
        
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
};

bool ItemRepository::Delete(const Item &_Item) 
{

};