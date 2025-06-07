#pragma once
#include <memory>
#include "../Repositories/ItemRepository.hpp"
#include "../Interfaces/IItemRepository.hpp"

enum Filter {
    ItemCode,
    ItemName,
    Codebars
};

class ItemService {
public:
    explicit ItemService(std::shared_ptr<IItemRepository> repository);

    bool AddItem(const ItemModel&);
    bool ModifyItem(const ItemModel&);
    std::vector<ItemModel> GetAllItems();
    ItemModel SearchItem(const int&);
    std::vector<ItemModel> SearchItem(enum Filter, const std::string&);

private:
    std::shared_ptr<IItemRepository> repository;
};
