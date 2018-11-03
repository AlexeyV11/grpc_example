#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "dataexchange.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ClientReader;


// grpc client

class DataExchangeClient 
{
public:

    DataExchangeClient(std::shared_ptr<Channel> channel)
    : stub_(DataExchange::NewStub(channel)) 
    {
    }

    int ReceiveNumber() 
    {
        Empty request;
        NumberReply reply;

        ClientContext context;
        Status status = stub_->GetNumber(&context, request, &reply);

        if (status.ok())
            return reply.value();

        throw std::runtime_error("server error");

    }

    std::string ReceiveString() 
    {
        Empty request;
        StringReply reply;
        ClientContext context;

        Status status = stub_->GetString(&context, request, &reply);

        if (status.ok())
            return reply.value();

        throw std::runtime_error("server error");
    }

    size_t ReceiveFile(const std::string& filename) 
    {
        ClientContext context;
        Empty empty;

        std::unique_ptr<grpc::ClientReader<PartReply> > reader(
                stub_->GetFile(&context, empty));

        PartReply part;

        size_t cnt = 0;
        while (reader->Read(&part)) 
        {
            cnt += part.value().size();
        }

        Status status = reader->Finish();
        if (status.ok()) 
        {
            return cnt;
        }

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

        std::cout << "Number received: " << client.ReceiveNumber() << std::endl;
        std::cout << "String received: " << client.ReceiveString() << std::endl;
        std::cout << "File received with size: " << client.ReceiveFile("sendme_dst.txt") << std::endl;

    }    
    catch (std::exception const& e) 
    {
        std::cout << "Exception: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
