#pragma once

#include <graphqlservice/GraphQLService.h>

namespace graphql::schema {
    class Schema;
}

class SchemaRequest : public graphql::service::Request
{
public:
    SchemaRequest(
        graphql::service::TypeMap operationTypes,
        std::shared_ptr<graphql::schema::Schema> schema)
        : graphql::service::Request(std::move(operationTypes), std::move(schema))
    {
    }
};
