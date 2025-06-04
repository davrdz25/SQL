#include "ItemWrapper.hpp"

std::shared_ptr<graphql::item::object::Item> wrapItemModel(const ItemModel& model)
{
    return std::make_shared<graphql::item::object::Item>(
        std::make_shared<ItemResolver>(model)
    );
}
