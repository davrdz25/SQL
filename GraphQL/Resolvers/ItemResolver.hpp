#pragma once

#include "../Generated/Item/ItemObject.h"
#include "../../Models/ItemModel.hpp"

class ItemResolver {
public:
    explicit ItemResolver(const ItemModel& model)
        : m_model(model) {}

    graphql::service::AwaitableScalar<std::string> getName(graphql::service::FieldParams&&) const {
        co_return m_model.ItemName;
    }

    graphql::service::AwaitableScalar<std::string> getCode(graphql::service::FieldParams&&) const {
        co_return m_model.ItemCode;
    }

    graphql::service::AwaitableScalar<std::string> getCodebars(graphql::service::FieldParams&&) const {
        co_return m_model.Codebars;
    }

    graphql::service::AwaitableScalar<double> getOnHand(graphql::service::FieldParams&&) const {
        co_return static_cast<double>(m_model.OnHand);
    }

    void beginSelectionSet(const graphql::service::SelectionSetParams&) const {}
    void endSelectionSet(const graphql::service::SelectionSetParams&) const {}

private:
    ItemModel m_model;
};
