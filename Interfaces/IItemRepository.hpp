#pragma once
#include <optional>
#include <vector>
#include "IBaseRepository.hpp"
#include "../Models/Item.hpp"

class IItemRepository : public IBaseRepository<Item> {
    public:
        ~IItemRepository() = default;
        virtual std::optional<Item> ReadByName(const std::string&) = 0;
        virtual std::optional<Item> ReadByCodebars(const std::string&) = 0;
};