// GraphQL/SchemaRequest.h
#pragma once

#include <graphqlservice/GraphQLService.h>

namespace graphql::service {

class SchemaRequest : public Request {
public:
    SchemaRequest(TypeMap operationTypes, std::shared_ptr<schema::Schema> schema)
        : Request(std::move(operationTypes), std::move(schema)) {}
};

} // namespace graphql::service
