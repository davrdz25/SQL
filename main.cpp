#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>

#include "GraphQL/Generated/Item/QueryObject.h"
#include "GraphQL/Resolvers/ItemQueryResolver.hpp"
#include "GraphQL/Resolvers/SchemaRequest.hpp"
#include <graphqlservice/GraphQLService.h>
#include <graphqlservice/JSONResponse.h>

#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace json = boost::json;
using tcp = boost::asio::ip::tcp;

using namespace graphql;
using namespace graphql::item;

http::response<http::string_body> handleGraphQLRequest(const std::string &body)
{
    auto jv = json::parse(body);
    auto obj = jv.as_object();

    std::string query = obj["query"].as_string().c_str();
    std::string operationName;
    if (obj.contains("operationName") && !obj["operationName"].is_null())
    {
        operationName = obj["operationName"].as_string().c_str();
    }

    // Construir variables como response::Value (vacío por defecto)
    response::Value variables{response::Type::Map};

    if (obj.contains("variables") && !obj["variables"].is_null())
    {
        // Parseamos variables JSON y convertimos a response::Value
        // Aquí convertimos boost::json::object a response::Value recursivamente

        std::function<response::Value(const json::value &)> convertJsonToResponseValue;
        convertJsonToResponseValue = [&](const json::value &v) -> response::Value
        {
            switch (v.kind())
            {
            case json::kind::object:
            {
                response::Value map{response::Type::Map};
                for (auto &[key, val] : v.as_object())
                {
                    map.emplace_back(std::string(key), convertJsonToResponseValue(val));
                }
                return map;
            }
            case json::kind::array:
            {
                response::Value arr{response::Type::List};
                for (auto &elem : v.as_array())
                {
                    arr.emplace_back(convertJsonToResponseValue(elem));
                }
                return arr;
            }
            case json::kind::string:
                return response::Value{std::string(v.as_string().c_str())};
            case json::kind::int64:
                return response::Value{static_cast<response::IntType>(v.as_int64())};
            case json::kind::uint64:
                return response::Value{static_cast<response::IntType>(v.as_uint64())};
            case json::kind::double_:
                return response::Value{v.as_double()};
            case json::kind::bool_:
                return response::Value{v.as_bool()};
            case json::kind::null:
            default:
                return response::Value{};
            }
        };

        variables = convertJsonToResponseValue(obj["variables"]);
    }

    auto queryRoot = std::make_shared<object::Query>(std::make_shared<ItemQueryResolver>());
    service::TypeMap operationTypes;
    operationTypes.emplace("Query", queryRoot);

    auto schema = GetSchema();

    // Parsear la consulta GraphQL a AST
    auto ast = graphql::peg::parseString(query);

    // Crear la petición GraphQL
    auto request = std::make_shared<graphql::service::SchemaRequest>(
        std::move(operationTypes), std::move(schema));
    

    // Construir los parámetros para resolve
    service::RequestResolveParams params{
        ast,
        std::string_view{operationName},
        std::move(variables),
        {}};

    // Ejecutar la consulta GraphQL y esperar resultado
    response::AwaitableValue futureResponse = request->resolve(params);
    response::Value result = futureResponse.get();

    // Serializar respuesta JSON
    std::string responseBody = graphql::response::toJSON(std::move(result));

    http::response<http::string_body> res{http::status::ok, 11};
    res.set(http::field::content_type, "application/json");
    res.body() = responseBody;
    res.prepare_payload();

    return res;
}

void do_session(tcp::socket socket)
{
    try
    {
        beast::flat_buffer buffer;

        // Leer request
        http::request<http::string_body> req;
        http::read(socket, buffer, req);

        // Procesar sólo POST a /graphql
        if (req.method() == http::verb::post && req.target() == "/graphql")
        {
            auto response = handleGraphQLRequest(req.body());
            http::write(socket, response);
        }
        else
        {
            // Responder 404
            http::response<http::string_body> res{http::status::not_found, req.version()};
            res.set(http::field::content_type, "text/plain");
            res.body() = "Not Found\n";
            res.prepare_payload();
            http::write(socket, res);
        }
    }
    catch (std::exception const &e)
    {
        std::cerr << "Session error: " << e.what() << std::endl;
    }
}

int main()
{
    try
    {
        net::io_context ioc{1};
        tcp::acceptor acceptor{ioc, tcp::endpoint(tcp::v4(), 8080)};
        std::cout << "Servidor GraphQL escuchando en http://localhost:8080/graphql\n";

        while (true)
        {
            tcp::socket socket{ioc};
            acceptor.accept(socket);
            do_session(std::move(socket));
        }
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error en servidor: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
