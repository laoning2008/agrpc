
#include "helper/awaitable_client_rpc.hpp"
#include "helper/helper.hpp"
#include "helper/rethrow_first_arg.hpp"

#include "helloworld.grpc.pb.h"


#include <agrpc/asio_grpc.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>

int main(int argc, const char** argv)
{
    const auto port = argc >= 2 ? argv[1] : "50051";
    const auto host = std::string("localhost:") + port;

    grpc::Status status;

    helloworld::Greeter::Stub stub{grpc::CreateChannel(host, grpc::InsecureChannelCredentials())};
    agrpc::GrpcContext grpc_context;

    asio::co_spawn(
        grpc_context,
        [&]() -> asio::awaitable<void>
        {
            using RPC = example::AwaitableClientRPC<&helloworld::Greeter::Stub::PrepareAsyncSayHello>;
            grpc::ClientContext client_context;
            helloworld::HelloRequest request;
            request.set_name("world");
            helloworld::HelloReply response;
            status = co_await RPC::request(grpc_context, stub, client_context, request, response);
            std::cout << status.ok() << " response: " << response.message() << std::endl;
        },
        example::RethrowFirstArg{});

    grpc_context.run();

    abort_if_not(status.ok());
}
