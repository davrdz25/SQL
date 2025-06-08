#pragma once

#include "../Generated/User/UserObject.h"
#include "../../Models/UserModel.hpp"

class UserResolver {
public:
    explicit UserResolver(const UserModel& model)
        : m_model(model) {}

    graphql::service::AwaitableScalar<int> getEntry(graphql::service::FieldParams&&) const {
        co_return m_model.Entry;
    }

    graphql::service::AwaitableScalar<std::string> getCode(graphql::service::FieldParams&&) const {
        co_return m_model.Code;
    }

    graphql::service::AwaitableScalar<std::string> getFirstName(graphql::service::FieldParams&&) const {
        co_return m_model.FirstName;
    }

    graphql::service::AwaitableScalar<std::string> getLastName(graphql::service::FieldParams&&) const {
        co_return m_model.LastName;
    }

    graphql::service::AwaitableScalar<std::string> getMiddleName(graphql::service::FieldParams&&) const {
        co_return m_model.MiddleName;
    }

    graphql::service::AwaitableScalar<std::string> getEmail(graphql::service::FieldParams&&) const {
        co_return m_model.Email;
    }

    graphql::service::AwaitableScalar<std::string> getPhone(graphql::service::FieldParams&&) const {
        co_return m_model.Phone;
    }

    graphql::service::AwaitableScalar<std::string> getPassword(graphql::service::FieldParams&&) const {
        co_return m_model.Password;
    }

    void beginSelectionSet(const graphql::service::SelectionSetParams&) const {}
    void endSelectionSet(const graphql::service::SelectionSetParams&) const {}

private:
    UserModel m_model;
};
