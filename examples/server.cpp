#include "helper/awaitable_server_rpc.hpp"
#include "helper/rethrow_first_arg.hpp"

#include "helloworld.grpc.pb.h"

#include <agrpc/asio_grpc.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/signal_set.hpp>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

// begin-snippet: server-side-helloworld
// ---------------------------------------------------
// Server-side hello world which handles exactly one request from the client before shutting down.
// ---------------------------------------------------
// end-snippet
int main(int argc, const char** argv)
{
    const auto port = argc >= 2 ? argv[1] : "50051";
    const auto host = std::string("0.0.0.0:") + port;

    helloworld::Greeter::AsyncService service;
    std::unique_ptr<grpc::Server> server;

    grpc::ServerBuilder builder;
    agrpc::GrpcContext grpc_context{builder.AddCompletionQueue()};
    builder.AddListeningPort(host, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    server = builder.BuildAndStart();

    using RPC = example::AwaitableServerRPC<&helloworld::Greeter::AsyncService::RequestSayHello>;
    agrpc::register_awaitable_rpc_handler<RPC>(
        grpc_context, service,
        [&](RPC& rpc, RPC::Request& request) -> asio::awaitable<void>
        {
            helloworld::HelloReply response;
            response.set_message("Hello " + request.name());
            co_await rpc.finish(response, grpc::Status::OK);
            server->Shutdown();
        },
        example::RethrowFirstArg{});

    grpc_context.run();
}
