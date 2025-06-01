#include <boost/asio/io_context.hpp>
#include <gtest/gtest.h>

enum : unsigned short { TCP_PORT = 50000 };

#ifdef BUILD_CLIENT
#include "tcp/client.hpp"

TEST(TCPClient, Initialization)
{
    boost::asio::io_context io_context;
    auto client = TCP::Client::create(io_context, "localhost", TCP_PORT);
    client->connect();
    io_context.run();
}

#ifdef BUILD_SERVER
#include "tcp/server.hpp"

TEST(TCP_Server, Initialization)
{
    boost::asio::io_context io_context;

    auto server = TCP::Server::create(io_context, TCP_PORT);

    io_context.run();
}

TEST(TCP_Server_Client, Connection)
{
    boost::asio::io_context io_context;

    auto server = TCP::Server::create(io_context, TCP_PORT);
    auto client = TCP::Client::create(io_context, "localhost", TCP_PORT);
    client->connect();

    io_context.run();
}

#endif //BUILD_SERVER

#endif //BUILD_CLIENT
