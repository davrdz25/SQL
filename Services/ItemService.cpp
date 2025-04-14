#include "ItemService.hpp"

bool ItemService::CreateItem(const Item& item){
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

        repository_->Create(item);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}