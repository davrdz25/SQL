#pragma once
#include "../../Models/Item.hpp"
#include <vector>
#include <memory>

class QueryResolver {
public:
    std::vector<std::shared_ptr<Item>> getItems() const {
        return {
            std::make_shared<Item>(Item{1, "Laptop", "LP123", "7894561230", 10.0f}),
            std::make_shared<Item>(Item{2, "Mouse", "MS456", "3216549870", 5.0f})
        };
    }
};