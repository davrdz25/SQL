#pragma once

#include "UserResolver.hpp"
#include "../Generated/User/UserMutationObject.h"
#include "../../Services/User/UserService.hpp"
#include "../../Models/UserModel.hpp"

#include <memory>
#include <optional>

class UserMutationResolver
{
public:
    explicit UserMutationResolver(std::shared_ptr<UserService> userService)
        : m_userService(std::move(userService)) {}

    graphql::service::AwaitableObject<std::optional<bool>> getModifyPassword(graphql::service::FieldParams &&params, int entry, std::string currentPassword, std::string newPassword) const
    {
        bool success = m_userService->ModifyPassword(entry, currentPassword, newPassword);
        co_return success;
    }

    graphql::service::AwaitableObject<std::shared_ptr<graphql::user::object::User>> getCreateUser(
    graphql::service::FieldParams&& params,
    std::string code,
    std::string firstName,
    std::optional<std::string> middleName,
    std::string lastName,
    std::string email,
    std::string phone,
    std::string password) const

    {
        UserModel model;
        model.Code = code;
        model.FirstName = firstName;
        model.MiddleName = middleName.value();
        model.LastName = lastName;
        model.Email = email;
        model.Phone = phone;
        model.Password = password;

        bool success = m_userService->AddUser(model);
        if (!success)
        {
            co_return nullptr;
        }

        auto resolver = std::make_shared<UserResolver>(model);
        co_return std::make_shared<graphql::user::object::User>(std::move(resolver));
    }

    graphql::service::AwaitableObject<std::shared_ptr<graphql::user::object::User>> getUpdateUser(graphql::service::FieldParams &&, int entry, std::optional<std::string> email, std::optional<std::string> phone) const
    {
        UserModel model = m_userService->GetUserByEntry(entry);

        if (email)
        {
            model.Email = *email;
        }
        if (phone)
        {
            model.Phone = *phone;
        }

        bool success = m_userService->ModifyUser(model);

        if (!success)
        {
            co_return nullptr;
        }

        auto resolver = std::make_shared<UserResolver>(model);
        co_return std::make_shared<graphql::user::object::User>(std::move(resolver));
    }

private:
    std::shared_ptr<UserService> m_userService;
};