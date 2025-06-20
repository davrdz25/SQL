#pragma once

#include <graphqlservice/GraphQLService.h>
#include "../Generated/User/UserQueryObject.h"
#include "UserPublicResolver.hpp"
#include "../../Services/User/UserService.hpp"
#include "../../DTOs/UserDTO.hpp"

#include <memory>
#include <vector>

class UserQueryResolver
{
public:
    explicit UserQueryResolver(std::shared_ptr<UserService> userService)
        : m_userService(std::move(userService)) {}

    graphql::service::AwaitableObject<std::shared_ptr<graphql::user::object::UserPublic>> getGetUser(graphql::service::FieldParams&& params,int entry) const
    {
        auto model = m_userService->GetUserByEntry(entry);

        auto resolver = std::make_shared<UserPublicResolver>(model);
        co_return std::make_shared<graphql::user::object::UserPublic>(std::move(resolver));
    }

    graphql::service::AwaitableObject<std::vector<std::shared_ptr<graphql::user::object::UserPublic>>> getGetUsers(graphql::service::FieldParams&& params, const graphql::user::UserInput &user) const
    {
        std::vector<std::shared_ptr<graphql::user::object::UserPublic>> gqlUsers;

        for (const auto &model : m_userService->GetAllUsers())
        {
            auto resolver = std::make_shared<UserPublicResolver>(model);
            gqlUsers.emplace_back(std::make_shared<graphql::user::object::UserPublic>(std::move(resolver)));
        }

        co_return gqlUsers;
    }

private:
    std::shared_ptr<UserService> m_userService;
};