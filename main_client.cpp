#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "dataexchange.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;


// grpc client
class DataExchangeClient 
{
public:

    DataExchangeClient(std::shared_ptr<Channel> channel)
    : stub_(DataExchange::NewStub(channel)) 
    {
    }

    int GetNumber() 
    {
        Empty request;
        NumberReply reply;

        ClientContext context;
        Status status = stub_->GetNumber(&context, request, &reply);

        if (status.ok())
            return reply.value();
        
        throw std::runtime_error("server error");
        
    }

    std::string GetString() 
    {
        Empty request;
        StringReply reply;
        ClientContext context;

        Status status = stub_->GetString(&context, request, &reply);

        if (status.ok())
            return reply.value();
        
        throw std::runtime_error("server error");
    }
private:
    std::unique_ptr<DataExchange::Stub> stub_;
};

int main(int argc, char** argv) 
{
    try
    {
        DataExchangeClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    
        std::cout << "Number received: " << client.GetNumber() << std::endl;
        std::cout << "String received: " << client.GetString() << std::endl;
    }
    catch(std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
        return 1;
    } 
    
    return 0;
}
