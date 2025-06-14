// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "UserMutationObject.h"
#include "UserObject.h"

#include "graphqlservice/internal/Schema.h"

#include "graphqlservice/introspection/IntrospectionSchema.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using namespace std::literals;

namespace graphql::user {
namespace object {

UserMutation::UserMutation(std::unique_ptr<const Concept> pimpl) noexcept
	: service::Object{ getTypeNames(), getResolvers() }
	, _pimpl { std::move(pimpl) }
{
}

service::TypeNames UserMutation::getTypeNames() const noexcept
{
	return {
		R"gql(UserMutation)gql"sv
	};
}

service::ResolverMap UserMutation::getResolvers() const noexcept
{
	return {
		{ R"gql(CreateUser)gql"sv, [this](service::ResolverParams&& params) { return resolveCreateUser(std::move(params)); } },
		{ R"gql(UpdateUser)gql"sv, [this](service::ResolverParams&& params) { return resolveUpdateUser(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } },
		{ R"gql(ModifyPassword)gql"sv, [this](service::ResolverParams&& params) { return resolveModifyPassword(std::move(params)); } }
	};
}

void UserMutation::beginSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->beginSelectionSet(params);
}

void UserMutation::endSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->endSelectionSet(params);
}

service::AwaitableResolver UserMutation::resolveCreateUser(service::ResolverParams&& params) const
{
	auto argCode = service::ModifiedArgument<std::string>::require("Code", params.arguments);
	auto argFirstName = service::ModifiedArgument<std::string>::require("FirstName", params.arguments);
	auto argMiddleName = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("MiddleName", params.arguments);
	auto argLastName = service::ModifiedArgument<std::string>::require("LastName", params.arguments);
	auto argEmail = service::ModifiedArgument<std::string>::require("Email", params.arguments);
	auto argPhone = service::ModifiedArgument<std::string>::require("Phone", params.arguments);
	auto argPassword = service::ModifiedArgument<std::string>::require("Password", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	service::SelectionSetParams selectionSetParams { static_cast<const service::SelectionSetParams&>(params) };
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getCreateUser(service::FieldParams { std::move(selectionSetParams), std::move(directives) }, std::move(argCode), std::move(argFirstName), std::move(argMiddleName), std::move(argLastName), std::move(argEmail), std::move(argPhone), std::move(argPassword));
	resolverLock.unlock();

	return service::ModifiedResult<User>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver UserMutation::resolveUpdateUser(service::ResolverParams&& params) const
{
	auto argEntry = service::ModifiedArgument<int>::require("Entry", params.arguments);
	auto argFirstName = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("FirstName", params.arguments);
	auto argMiddleName = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("MiddleName", params.arguments);
	auto argLastName = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("LastName", params.arguments);
	auto argEmail = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("Email", params.arguments);
	auto argPhone = service::ModifiedArgument<std::string>::require<service::TypeModifier::Nullable>("Phone", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	service::SelectionSetParams selectionSetParams { static_cast<const service::SelectionSetParams&>(params) };
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getUpdateUser(service::FieldParams { std::move(selectionSetParams), std::move(directives) }, std::move(argEntry), std::move(argFirstName), std::move(argMiddleName), std::move(argLastName), std::move(argEmail), std::move(argPhone));
	resolverLock.unlock();

	return service::ModifiedResult<User>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver UserMutation::resolveModifyPassword(service::ResolverParams&& params) const
{
	auto argEntry = service::ModifiedArgument<int>::require("Entry", params.arguments);
	auto argOldPassword = service::ModifiedArgument<std::string>::require("OldPassword", params.arguments);
	auto argNewPassword = service::ModifiedArgument<std::string>::require("NewPassword", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	service::SelectionSetParams selectionSetParams { static_cast<const service::SelectionSetParams&>(params) };
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getModifyPassword(service::FieldParams { std::move(selectionSetParams), std::move(directives) }, std::move(argEntry), std::move(argOldPassword), std::move(argNewPassword));
	resolverLock.unlock();

	return service::ModifiedResult<bool>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver UserMutation::resolve_typename(service::ResolverParams&& params) const
{
	return service::Result<std::string>::convert(std::string{ R"gql(UserMutation)gql" }, std::move(params));
}

} // namespace object

void AddUserMutationDetails(const std::shared_ptr<schema::ObjectType>& typeUserMutation, const std::shared_ptr<schema::Schema>& schema)
{
	typeUserMutation->AddFields({
		schema::Field::Make(R"gql(CreateUser)gql"sv, R"md()md"sv, std::nullopt, schema->LookupType(R"gql(User)gql"sv), {
			schema::InputValue::Make(R"gql(Code)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(FirstName)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(MiddleName)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(LastName)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(Email)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(Phone)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(Password)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(UpdateUser)gql"sv, R"md()md"sv, std::nullopt, schema->LookupType(R"gql(User)gql"sv), {
			schema::InputValue::Make(R"gql(Entry)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(Int)gql"sv)), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(FirstName)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(MiddleName)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(LastName)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(Email)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(Phone)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(ModifyPassword)gql"sv, R"md()md"sv, std::nullopt, schema->LookupType(R"gql(Boolean)gql"sv), {
			schema::InputValue::Make(R"gql(Entry)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(Int)gql"sv)), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(OldPassword)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(NewPassword)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv)
		})
	});
}

} // namespace graphql::user
