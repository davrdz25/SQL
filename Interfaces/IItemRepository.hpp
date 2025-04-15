#pragma once
#include <optional>
#include <vector>
#include "IBaseRepository.hpp"
#include "../Models/Item.hpp"

class IItemRepository : public IBaseRepository<Item> {
    public:
        ~IItemRepository() = default;
        virtual std::vector<Item> ReadByName(const std::string&) = 0;
        virtual std::vector<Item> ReadByCodebars(const std::string&) = 0;
        virtual std::vector<Item> ReadByCode(const std::string&) = 0;

};