// services/ItemService.hpp
#pragma once
#include <memory>
#include "../Repositories/ItemRepository.hpp"
#include "../Interfaces/IItemRepository.hpp"

class ItemService {
public:
    explicit ItemService(std::shared_ptr<IItemRepository> repository)
        : repository_(std::move(repository)) {};

    bool CreateItem(const Item& item);

    std::optional<Item> findItem(int id) {
        return repository_->ReadAll();
    }

private:
    std::shared_ptr<IItemRepository> repository_;
};
