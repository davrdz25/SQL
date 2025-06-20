#pragma once

#include <graphqlservice/GraphQLService.h>
#include <memory>
#include <optional>
#include <string>
#include "UserPublicResolver.hpp"
#include "AuthResolver.hpp"
#include "../../Generated/User/UserMutationObject.h"
#include "../../../Services/User/UserService.hpp"
#include "../../../DTOs/CreateUserDTO.hpp"
#include "../../../DTOs/ChangePasswordDTO.hpp"
#include "../../../Models/UserModel.hpp"
#include "../../../Models/UserPublicModel.hpp"
#include "../../../Models/AuthPayloadModel.hpp"

class UserMutationResolver
{
private:
    std::shared_ptr<UserService> m_userService;

public:
    explicit UserMutationResolver(std::shared_ptr<UserService> userService) noexcept
        : m_userService(std::move(userService))
    {
    }

    graphql::service::AwaitableObject<std::shared_ptr<graphql::user::object::UserPublic>> getCreateUser(graphql::service::FieldParams &&params, const graphql::user::CreateUserInput &newUser) const
    {
        try
        {
            if (newUser.Code == "")
                throw std::runtime_error("Code cannot be empty");

            if (newUser.FirstName == "")
                throw std::runtime_error("First name cannot be empty");

            if (newUser.LastName== "")
                throw std::runtime_error("Last name cannot be empty");

            if (newUser.Email == "")
                throw std::runtime_error("Email cannot be empty");

            if (newUser.Phone == "")
                throw std::runtime_error("Phone cannot be empty");

            if (newUser.Password == "")
                throw std::runtime_error("Password cannot be empty");
            
            const CreateUserDTO userParam{
                newUser.Code,
                newUser.FirstName,
                newUser.MiddleName,
                newUser.LastName,
                newUser.SecondLastName,
                newUser.Email,
                newUser.Phone,
                newUser.Password
            };

            m_userService->AddUser(userParam);
            
            auto userModel = m_userService->GetUserByCode(newUser.Code);

            auto resolver = std::make_shared<UserPublicResolver>(userModel);

            co_return std::make_shared<graphql::user::object::UserPublic>(std::move(resolver));

        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            throw std::runtime_error(e.what());
        }
    }

    graphql::service::AwaitableObject<std::shared_ptr<graphql::user::object::UserPublic>> getUpdateUser(graphql::service::FieldParams &&params, const graphql::user::UpdateUserInput &userChanges) const
    {
        try
        {
            if (!userChanges.FirstName.has_value() &&
                !userChanges.MiddleName.has_value() &&
                !userChanges.LastName.has_value() &&
                !userChanges.SecondLastName.has_value() &&
                !userChanges.Email.has_value() &&
                !userChanges.Phone.has_value()
            )
                throw std::runtime_error("No fields to update");

            auto userModel = m_userService->GetUserByEntry(1);

            auto resolver = std::make_shared<UserPublicResolver>(userModel);

            co_return std::make_shared<graphql::user::object::UserPublic>(std::move(resolver));
            
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            throw std::runtime_error(e.what());
        }
    }

    graphql::service::AwaitableScalar<bool> getModifyPassword(graphql::service::FieldParams &&params, const graphql::user::ChangePasswordInput &user) const
    {
        try
        {
            if(user.Code == "")
                throw std::runtime_error("Code cannot be empty");
            
            if(user.OldPassword == user.NewPassword)
                throw std::runtime_error("New password not to be equal than old password");

            return false;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            throw std::runtime_error(e.what());
        }
    }

    graphql::service::AwaitableObject<std::shared_ptr<graphql::user::object::AuthPayload>> getLoginUser(graphql::service::FieldParams &&params, const  graphql::user::UserSession &dto) const
    {
        try
        {
            const UserSessionDTO userSession{
                dto.Code.value(),
                dto.Email.value(),
                dto.Phone.value(),
                dto.Password
            };

            auto model = m_userService->CreateUserSession(userSession);
            auto resolver = std::make_shared<AuthPayloadResolver>(model);

            co_return std::make_shared<graphql::user::object::AuthPayload>(std::move(resolver));

        }
        catch(const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    };
};