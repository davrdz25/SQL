#include "ItemRepository.hpp"

ItemRepository::ItemRepository(std::shared_ptr<SQL> Database): Database(std::move(Database)) {}

std::optional<std::vector<ItemModel>> ItemRepository::ReadByCode(const std::string &_ItemCode)
{
    try
    {
        DataTable dataTable;

        std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items WHERE ItemCode = ?";
        std::vector<std::string> params = {_ItemCode};

        dataTable.Fill(Database->FetchPrepared(Query, params));

        if (dataTable.RowsCount() > 0)
        {
            std::vector<ItemModel> items;

            for (int i = 0; i < dataTable.RowsCount(); i++)
            {
                items.push_back(ItemModel{
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

std::optional<std::vector<ItemModel>> ItemRepository::ReadByName(const std::string &_Name)
{
    try
    {
        std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items WHERE ItemName = '" + _Name + "'";

        DataTable dataTable;
        
        dataTable.Fill(Database->FetchResults(Query));

        if(dataTable.RowsCount() == 0)
            return std::nullopt;
            
        std::vector<ItemModel> items;

        std::cout << "Results length: " << std::to_string(dataTable.RowsCount()) << std::endl;

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
            items.push_back(ItemModel{
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

std::optional<std::vector<ItemModel>> ItemRepository::ReadByCodebars(const std::string &_Codebars)
{
    try
    {
        DataTable dataTable;

        std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items WHERE Codebars = '" + _Codebars + "'";

        dataTable.Fill(Database->FetchResults(Query));

        if (dataTable.RowsCount() == 0)
            return std::nullopt;

        std::vector<ItemModel> items;

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
            items.push_back(ItemModel{
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

std::optional<std::vector<ItemModel>> ItemRepository::ReadAll()
{
    try
    {
        DataTable dataTable;
        std::vector<ItemModel> items;

        std::string Query = "SELECT [Entry], ItemName, ItemCode, Codebars, OnHand FROM Items";

        std::cout << Query << std::endl;

        
        dataTable.Fill(Database->FetchResults(Query));

        if (dataTable.RowsCount() == 0)
            return std::nullopt;

        for (int i = 0; i < dataTable.RowsCount(); i++)
        {
            items.push_back(ItemModel{
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

std::optional<ItemModel> ItemRepository::ReadByEntry(const int &_Entry)
{
    try
    {
        DataTable dataTable;
        ItemModel item;

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

bool ItemRepository::Create(const ItemModel &_Item)
{
    try
    {
        std::string Query = "INSERT INTO Items ([Entry], ItemName, ItemCode, Codebars, OnHand) VALUES (?,?,?,?,?)";
        std::vector<std::string> params = {
            std::to_string(_Item.Entry), 
            _Item.ItemName, 
            _Item.ItemCode, 
            _Item.Codebars, 
            std::to_string(_Item.OnHand)
        };

        std::cout << Query << std::endl;

        if (Database->RunPrepared(Query, params))
            return true;

        return false;
    }
    catch (const std::exception &Ex)
    {
        std::runtime_error(Ex.what());
        return false;
    }
};

bool ItemRepository::Update(const ItemModel &_Item)
{
    try
    {
        std::string Query = "UPDATE Items SET ItemName = ?, ItemCode = ?, Codebars = ?, OnHand = ? WHERE [Entry] = ? ";
        std::vector<std::string> params = {
            _Item.ItemName, 
            _Item.ItemCode, 
            _Item.Codebars, 
            std::to_string(_Item.OnHand),
            std::to_string(_Item.Entry) 
        };

        std::cout << Query << std::endl;

        if (Database->RunPrepared(Query, params))
            return true;

        return false;
        
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
};

bool ItemRepository::Delete(const ItemModel &_Item) 
{
    try
    {
        std::string Query = "DELETE FROM Items WHERE [Entry] = ? ";
        std::vector<std::string> params = {
            std::to_string(_Item.Entry) 
        };

        std::cout << Query << std::endl;

        if (Database->RunPrepared(Query, params))
            return true;

        return false;
        
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
};