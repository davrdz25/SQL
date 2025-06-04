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
        
        std::optional<std::vector<ItemModel>> ReadAll() override;
        std::optional<std::vector<ItemModel>> ReadByName(const std::string&) override;
        std::optional<ItemModel> ReadByEntry(const int&) override;
        std::optional<std::vector<ItemModel>> ReadByCodebars(const std::string&) override;
        std::optional<std::vector<ItemModel>> ReadByCode(const std::string&) override;

        bool Create(const ItemModel&) override;
        bool Update(const ItemModel&) override;
        bool Delete(const ItemModel&) override;

    private:
        std::shared_ptr<SQL> Database;
};