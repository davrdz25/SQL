#pragma once
#include <optional>
#include <vector>
#include "IBaseRepository.hpp"
#include "../Models/ItemModel.hpp"

class IItemRepository : public IBaseRepository<ItemModel> {
    public:
        ~IItemRepository() = default;
        virtual std::optional<std::vector<ItemModel>> ReadByName(const std::string&) = 0;
        virtual std::optional<std::vector<ItemModel>> ReadByCodebars(const std::string&) = 0;
        virtual std::optional<std::vector<ItemModel>> ReadByCode(const std::string&) = 0;
};