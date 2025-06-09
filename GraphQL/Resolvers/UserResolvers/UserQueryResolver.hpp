#pragma once

#include <graphqlservice/GraphQLService.h>
#include "../Generated/User/UserQueryObject.h"
#include "UserResolver.hpp"
#include "../../Services/User/UserService.hpp"

#include <memory>
#include <vector>

class UserQueryResolver
{
public:
    explicit UserQueryResolver(std::shared_ptr<UserService> userService)
        : m_userService(std::move(userService)) {}

    graphql::service::AwaitableObject<std::shared_ptr<graphql::user::object::User>> getGetUser(
        int entry) const
    {
        // Aquí tu lógica para obtener un usuario
        auto model = m_userService->GetUserByEntry(entry);

        auto resolver = std::make_shared<UserResolver>(model);
        co_return std::make_shared<graphql::user::object::User>(std::move(resolver));
    }

    graphql::service::AwaitableObject<std::vector<std::shared_ptr<graphql::user::object::User>>> getGetUsers(
        graphql::service::FieldParams &&) const
    {
        std::vector<std::shared_ptr<graphql::user::object::User>> gqlUsers;

        for (const auto &model : m_userService->GetAllUsers())
        {
            auto resolver = std::make_shared<UserResolver>(model);
            gqlUsers.emplace_back(std::make_shared<graphql::user::object::User>(std::move(resolver)));
        }

        co_return gqlUsers;
    }

private:
    std::shared_ptr<UserService> m_userService;
};