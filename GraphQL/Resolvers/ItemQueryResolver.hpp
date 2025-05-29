// ItemQueryResolver.hpp
#pragma once

#include "../Generated/Item/QueryObject.h"
#include "../Generated/Item/ItemObject.h"
#include "ItemResolver.hpp"
#include "QueryResolver.hpp"

#include <memory>
#include <vector>

class ItemQueryResolver final {
public:
    void beginSelectionSet(const graphql::service::SelectionSetParams&) const {}
    void endSelectionSet(const graphql::service::SelectionSetParams&) const {}

    graphql::service::AwaitableObject<std::vector<std::shared_ptr<graphql::item::object::Item>>> getItems(graphql::service::FieldParams&&) const {
        QueryResolver resolver;
        auto items = resolver.getItems();

        std::vector<std::shared_ptr<graphql::item::object::Item>> gqlItems;
        for (const auto& item : items) {
            gqlItems.emplace_back(
                std::make_shared<graphql::item::object::Item>(
                    std::make_shared<ItemResolver>(item)
                )
            );
        }

        co_return gqlItems;
    }
};
