#pragma once

#include "graphqlservice/GraphQLService.h"
#include "../Generated/Item/ItemObject.h"
#include "../../Models/Item.hpp"

class ItemResolver {
public:
    explicit ItemResolver(std::shared_ptr<Item> item) : _item(std::move(item)) {}

    graphql::service::AwaitableScalar<std::string> getName() const {
        co_return _item->ItemName;
    }

    graphql::service::AwaitableScalar<std::string> getCode() const {
        co_return _item->ItemCode;
    }

    graphql::service::AwaitableScalar<std::string> getCodebars() const {
        co_return _item->Codebars;
    }

    graphql::service::AwaitableScalar<double> getOnHand() const {
        co_return _item->OnHand;
    }

private:
    std::shared_ptr<Item> _item;
};
