#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>

#include "dataexchange.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;


// grpc server
class DataExchangeServer final : public DataExchange::Service 
{
public:
        DataExchangeServer(int n, std::string str, std::string filepath)
    : m_n(n), m_str(str), m_filepath(filepath)
    {}

private:
    Status GetFile(ServerContext* context, const Empty* request, ServerWriter<PartReply>* writer) override {
        
    for(int i=0;i<10;++i)
    {
        std::string ss = "1234";
        
        PartReply f;
        f.set_value(ss);
        writer->Write(f);
    }
    
    return Status::OK;
  }
    
    Status GetNumber(ServerContext* context, const Empty* request, NumberReply* reply) override 
    {
        int a = 43;
        reply->set_value(a);
        return Status::OK;
    }
    
    Status GetString(ServerContext* context, const Empty* request, StringReply* reply) override 
    {
        std::string a("hello");
        reply->set_value(a);
        return Status::OK;
    }

    int m_n {0};
    std::string m_str {""};
    std::string m_filepath {""};
};

void RunServer(const std::string& net_address, int n, const std::string& str, const std::string& filename) 
{
    DataExchangeServer service(42, "hello", "send_me.txt");

    ServerBuilder builder;
    builder.AddListeningPort(net_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << net_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) 
{
    // TODO: add proper configuration here
    // TODO: add error handling
    RunServer("0.0.0.0:50051", 42, "hello", "sendme.txt");
    
    return 0;
}
