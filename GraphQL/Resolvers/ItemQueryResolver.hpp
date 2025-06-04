#pragma once

#include "../Generated/Item/QueryObject.h"
#include "../../Services/ItemService.hpp"
#include "ItemResolver.hpp"

#include <memory>
#include <vector>

class ItemQueryResolver {
public:
    explicit ItemQueryResolver(std::shared_ptr<ItemService> itemService)
        : m_itemService(std::move(itemService)) {}

    graphql::service::AwaitableObject<std::vector<std::shared_ptr<graphql::item::object::Item>>> getItems(
        graphql::service::FieldParams&& params) const
    {
        std::vector<std::shared_ptr<graphql::item::object::Item>> gqlItems;

        for (const auto& item : m_itemService->GetAllItems()) {
            ItemModel model{
                item.Entry,
                item.ItemName,
                item.ItemCode,
                item.Codebars,
                item.OnHand
            };

            auto resolver = std::make_shared<ItemResolver>(model);

            // CONSTRUCTOR que toma el resolver
            gqlItems.emplace_back(std::make_shared<graphql::item::object::Item>(std::move(resolver)));
        }

        co_return gqlItems;
    }

private:
    std::shared_ptr<ItemService> m_itemService;
};
