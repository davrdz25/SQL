// services/ItemService.hpp
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

    bool AddItem(const Item& item);
    bool ModifyItem(const Item& item);
    std::optional<std::vector<Item>> GetAllItems();
    std::optional<Item> SearchItem(const int&);
    std::optional<std::vector<Item>> SearchItem(enum Filter, const std::string&);

private:
    std::shared_ptr<IItemRepository> repository;
};
