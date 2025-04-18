#pragma once
#include <optional>
#include <vector>
#include "../Interfaces/IItemRepository.hpp"
#include "../Database/SQL.hpp"
#include "../Database/DataTable.hpp"

class ItemRepository : public IItemRepository {
    public:
        explicit ItemRepository(std::shared_ptr<SQL> Database);

        ~ItemRepository(){};
        
        std::optional<std::vector<Item>> ReadAll() override;
        std::optional<std::vector<Item>> ReadByName(const std::string&) override;
        std::optional<Item> ReadByEntry(const int&) override;
        std::optional<std::vector<Item>> ReadByCodebars(const std::string&) override;
        std::optional<std::vector<Item>> ReadByCode(const std::string&) override;

        bool Create(const Item&) override;
        bool Update(const Item&) override;
        bool Delete(const Item&) override;

    private:
        std::shared_ptr<SQL> Database;
};