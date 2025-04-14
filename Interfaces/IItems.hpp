#pragma once
#include <optional>
#include <vector>
#include "IBaseRepository.hpp"
#include "../Models/Item.hpp"

template <typename T>

class IItem : public BaseRepository<T> {
    public:
        ~ItemRepository() = default;
        virtual std::optional<Item> ReadByName(std::string& const) = 0;
        virtual std::optional<Item> ReadByCodebars(std::string& const) = 0;
};