#pragma once

#include <graphqlservice/GraphQLService.h>
#include <memory>
#include <optional>
#include <string>
#include "../../Generated/User/UserMutationObject.h"
#include "UserResolver.hpp"
#include "../../../Services/User/UserService.hpp"
#include "../../../Models/UserModel.hpp"

class UserMutationResolver
{
private:
    std::shared_ptr<UserService> m_userService;

public:
    explicit UserMutationResolver(std::shared_ptr<UserService> userService) noexcept
        : m_userService(std::move(userService))
    {
    }

    graphql::service::AwaitableObject<std::shared_ptr<graphql::user::object::User>> getCreateUser(
        graphql::service::FieldParams &&params,
        std::string _Code,
        std::string _FirstName,
        std::optional<std::string> _MiddleName,
        std::string _LastName,
        std::string _Email,
        std::string _Phone,
        std::string _Password) const
    {
        try
        {
            UserModel userModel{
                .Code = std::move(_Code),
                .FirstName = std::move(_FirstName),
                .MiddleName = std::move(_MiddleName.value()),
                .LastName = std::move(_LastName),
                .Email = std::move(_Email),
                .Phone = std::move(_Phone),
                .Password = std::move(_Password),
            };

            auto model = m_userService->AddUser(userModel);
            auto resolver = std::make_shared<UserResolver>(userModel);

            co_return std::make_shared<graphql::user::object::User>(std::move(resolver));
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            throw std::runtime_error(e.what());
        }
    }

    graphql::service::AwaitableObject<std::shared_ptr<graphql::user::object::User>>
    getUpdateUser(
        graphql::service::FieldParams &&params,
        int _Entry,
        std::optional<std::string> _FirstName,
        std::optional<std::string> _MiddleName,
        std::optional<std::string> _LastName,
        std::optional<std::string> _Email,
        std::optional<std::string> _Phone) const
    {
        UserModel userModel{
            .Entry = std::move(_Entry),

        };

        auto model = m_userService->ModifyUser(userModel);

        auto resolver = std::make_shared<UserResolver>(userModel);
        co_return std::make_shared<graphql::user::object::User>(std::move(resolver));
    }

    graphql::service::AwaitableScalar<std::optional<bool>> getModifyPassword(
        graphql::service::FieldParams &&params,
        int Entry,
        std::string OldPassword,
        std::string NewPassword) const
    {
        auto result = m_userService->ModifyPassword(Entry, OldPassword, NewPassword);
        co_return result;
    }
};
