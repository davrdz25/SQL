#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>

#include <graphqlservice/GraphQLService.h>
#include <graphqlservice/JSONResponse.h>

#include <graphqlservice/internal/Grammar.h>
#include <graphqlservice/internal/SyntaxTree.h>


#include "GraphQL/Resolvers/UserResolvers/UserQueryResolver.hpp"
#include "GraphQL/Resolvers/UserResolvers/UserMutationResolver.hpp"

#include "GraphQL/Resolvers/RootQueryResolver.hpp"
#include "GraphQL/Resolvers/RootMutationResolver.hpp"

#include "GraphQL/Generated/User/MutationObject.h"
#include "GraphQL/Generated/User/QueryObject.h"


#include "Repositories/User/UserRepository.hpp"
#include "Services/User/UserService.hpp"

#include <iostream>
#include <memory>
#include <string>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>

#include <iostream>
#include <memory>
#include <string>
#include <functional>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
namespace net = boost::asio;
namespace json = boost::json;
namespace ssl = boost::asio::ssl;

using tcp = boost::asio::ip::tcp;

using namespace graphql;
//using namespace graphql::item;

boost::json::object astNodeToJson(const graphql::peg::ast_node *node)
{
    boost::json::object jsonNode;

    if (!node)
        return jsonNode;

    jsonNode["rule"] = node->type;

    if (node->has_content())
    {
        std::string content = std::string(node->string_view());
        if (!content.empty())
        {
            jsonNode["content"] = content;
        }
    }

    if (!node->children.empty())
    {
        boost::json::array children;
        for (const auto &child : node->children)
        {
            children.emplace_back(astNodeToJson(child.get()));
        }
        jsonNode["children"] = std::move(children);
    }

    return jsonNode;
}

void printNode(const graphql::peg::ast_node *node, int indent = 0)
{
    if (!node)
        return;

    std::string indentStr(indent * 2, ' ');
    std::cout << indentStr << "Rule: " << node->type << "\n";

    if (node->has_content())
    {
        const auto content = node->string_view();
        if (!content.empty())
        {
            std::cout << indentStr << "Content: " << content << "\n";
        }
    }

    for (const auto &child : node->children)
    {
        printNode(child.get(), indent + 1);
    }
}

http::response<http::string_body> handleGraphQLRequest(const std::string &body, std::shared_ptr<SQL> database)
{
    try
    {
        std::cout << "Request body:\n"
                  << body << "\n";

        boost::json::value jv = json::parse(body);
        boost::json::object obj = jv.as_object();

        std::string operationName;
        if (obj.contains("operationName") && !obj["operationName"].is_null())
        {
            operationName = obj["operationName"].as_string().c_str();
        }

        response::Value variables{response::Type::Map};
        if (obj.contains("variables") && !obj["variables"].is_null())
        {
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

        std::string schema = "Item";
        if (obj.contains("schema") && !obj["schema"].is_null())
        {
            schema = obj["schema"].as_string().c_str();
        }

        std::shared_ptr<graphql::service::Request> request;

        if (schema == "User")
        {
            auto userRepository = std::make_shared<UserRepository>(database);
            auto userService = std::make_shared<UserService>(userRepository);

            auto userQ = std::make_shared<UserQueryResolver>(userService);
            auto userM = std::make_shared<UserMutationResolver>(userService);

            auto rootQ = std::make_shared<graphql::resolvers::RootQueryResolver>(userQ);
            auto rootM = std::make_shared<graphql::resolvers::RootMutationResolver>(userM);

            auto qObj = std::shared_ptr<graphql::user::object::Query>(
                new graphql::user::object::Query(rootQ)
            );
            auto mObj = std::shared_ptr<graphql::user::object::Mutation>(new graphql::user::object::Mutation(rootM));

            auto request = std::make_shared<graphql::user::Operations>(
                qObj,
                mObj
            );
        }
        else
        {
            throw std::runtime_error("Schema no soportado");
        }

        std::string queryStr = obj["query"].as_string().c_str();
        std::cerr << "Ejecutando Query:\n"
                  << queryStr << "\n";

        graphql::peg::ast ast = graphql::peg::parseString(queryStr);

        if (!ast.root)
        {
            throw std::runtime_error("Error al parsear la consulta GraphQL.");
        }

        std::cout << "AST parseado correctamente\n";

        boost::json::object jsonAst = astNodeToJson(ast.root.get());
        std::cout << "AST (JSON):\n"
                  << boost::json::serialize(jsonAst) << "\n";

        service::RequestResolveParams params{
            ast,
            std::string_view{operationName},
            std::move(variables),
            graphql::service::await_async{}};

        // Ejecutar la consulta
        auto futureResult = request->resolve(params);
        auto result = futureResult.get();

        std::string responseBody = graphql::response::toJSON(std::move(result));

        http::response<http::string_body> res{http::status::ok, 11};
        res.set(http::field::content_type, "application/json");
        res.body() = responseBody;
        res.prepare_payload();

        return res;
    }
    catch (const std::exception &e)
    {
        http::response<http::string_body> res{http::status::bad_request, 11};
        res.set(http::field::content_type, "application/json");

        std::string errorJson = R"({"errors": [{"message": ")" + std::string(e.what()) + R"("}]})";
        res.body() = errorJson;
        res.prepare_payload();
        return res;
    }
}

void do_session(tcp::socket socket)
{
    try
    {
        websocket::stream<tcp::socket> ws{std::move(socket)};
        ws.accept();

        std::shared_ptr<SQL> Database = std::make_shared<SQL>();
        Database->ServerName("192.168.1.253");
        Database->UserName("sa");
        Database->Password("Development..");
        Database->DatabaseName("POS");
        Database->TrustServerCertificate(true);

        if (!Database->Connect())
        {
            std::cerr << "No se pudo conectar a la base de datos" << std::endl;
            return;
        }

        beast::flat_buffer buffer;

        while (true)
        {
            buffer.clear();
            ws.read(buffer);

            std::string message = beast::buffers_to_string(buffer.data());

            try
            {
                auto response = handleGraphQLRequest(message, Database);

                ws.text(true);
                ws.write(net::buffer(response.body()));
            }
            catch (const std::exception &e)
            {
                std::string errorJson = R"({"errors": [{"message": ")" + std::string(e.what()) + R"("}]})";
                ws.text(true);
                ws.write(net::buffer(errorJson));
            }
        }
    }
    catch (boost::beast::system_error const &se)
    {
        if (se.code() != websocket::error::closed)
        {
            std::cerr << "WebSocket session error: " << se.code().message() << std::endl;
        }
    }
    catch (std::exception const &e)
    {
        std::cerr << "Session error: " << e.what() << std::endl;
    }
}

void http_server(net::io_context &ioc, unsigned short port)
{
    try
    {
        tcp::acceptor acceptor{ioc, tcp::endpoint{tcp::v4(), port}};

        for (;;)
        {
            tcp::socket socket{ioc};
            acceptor.accept(socket);

            std::thread{[sock = std::move(socket)]() mutable
                        {
                            beast::flat_buffer buffer;
                            http::request<http::string_body> req;
                            http::read(sock, buffer, req);

                            auto database = std::make_shared<SQL>();
                            database->ServerName("192.168.1.253");
                            database->UserName("sa");
                            database->Password("Development..");
                            database->DatabaseName("POS");
                            database->TrustServerCertificate(true);
                            database->Connect();

                            auto res = handleGraphQLRequest(req.body(), database);

                            http::write(sock, res);
                            sock.shutdown(tcp::socket::shutdown_send);
                        }}
                .detach();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[HTTP Error] " << e.what() << "\n";
    }
}

void ws_server(net::io_context &ioc, unsigned short port)
{
    try
    {
        tcp::acceptor acceptor{ioc, tcp::endpoint{tcp::v4(), port}};
        for (;;)
        {
            tcp::socket socket{ioc};
            acceptor.accept(socket);

            std::thread{[s = std::move(socket)]() mutable
                        {
                            do_session(std::move(s));
                        }}
                .detach();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[WebSocket Error] " << e.what() << "\n";
    }
}

int main()
{
    try
    {
        net::io_context ioc;

        std::thread t_http([&ioc]()
                           {
                               http_server(ioc, 8081);
                               std::cerr << "[HTTP Server] Stopped.\n"; });

        std::thread t_ws([&ioc]()
                         {
                             ws_server(ioc, 9090);
                             std::cerr << "[WebSocket Server] Stopped.\n"; });

        t_http.join();
        t_ws.join();
    }
    catch (const std::exception &e)
    {
        std::cerr << "[FATAL] Exception: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}