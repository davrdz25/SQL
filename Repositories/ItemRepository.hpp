#pragma once
#include <optional>
#include <vector>
#include "../Interfaces/IItems.hpp"
template <typename T>

class ItemRepository : public IItem {
    public:
        ItemRepository();
        ~ItemRepository();
        std::optional<Item> ReadByName(std::string& const) override;
        std::optional<Item> ReadByCodebars(std::string& const) override;
        bool Create(const Item&) override;
        bool Update(const Item&) override;
        bool Delete(const Item&) override;
};