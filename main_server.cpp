#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>

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
    Status GetFile(ServerContext* context, const Empty* request, ServerWriter<PartReply>* writer) override 
    {
        
        std::ifstream fin(this->m_filepath, std::ifstream::binary);

        // chunk size
        const size_t buf_size = 1024 * 64;
        
        // typedef basic_string<char> string; sizeof(char) is always 1.
        // so we can use str as a byte buffer
        std::string str(buf_size, 'a');
        
        size_t total_count = 0;
        
        while (!fin.eof() && fin.good())
        { 
            fin.read(&str[0], str.size()); 
            size_t bytes_read = fin.gcount(); 

            // TODO: optimise this part to remove extra copy
            if (bytes_read == buf_size)
            {
                // send all the data
                PartReply f;
                f.set_value(str);
                writer->Write(f);
            }
            else
            {
                PartReply f;
                f.set_value(str.substr(0, bytes_read));
                writer->Write(f);
            }
            
            total_count += bytes_read;
        }
        fin.close();

        std::cout << "File sent with size :" << total_count << std::endl;
        return Status::OK;
    }
    
    Status GetNumber(ServerContext* context, const Empty* request, NumberReply* reply) override 
    {
        reply->set_value(m_n);
        std::cout << "Number sent :" << m_n << std::endl;
        return Status::OK;
    }
    
    Status GetString(ServerContext* context, const Empty* request, StringReply* reply) override 
    {
        reply->set_value(m_str);
        std::cout << "String sent :" << m_str << std::endl;
        return Status::OK;
    }

    int m_n {0};
    std::string m_str {""};
    std::string m_filepath {""};
};

void RunServer(const std::string& net_address, int n, const std::string& str, const std::string& filename) 
{
    DataExchangeServer service(n, str, filename);

    ServerBuilder builder;
    builder.AddListeningPort(net_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening :" << net_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) 
{
    // TODO: add better param parsing
    std::string param_message = "usage: main_server network_address[string] int_to_sent[int] string_to_send[string] file_to_send[string]";
    if (argc != 5)
    {
        std::cout << param_message << std::endl;
        return 1;
    }
    
    try 
    {
        std::string network_address = argv[1];
        std::string send_int_arg = argv[2];
        std::string send_string = argv[3];
        std::string send_file = argv[4];
        
        std::istringstream ss(send_int_arg);
        int send_int = -1;
        
        if (!(ss >> send_int)) 
            throw std::runtime_error("Invalid number :" + send_int_arg);
        else if(!ss.eof())
            throw std::runtime_error("Trailing characters after number :" + send_int_arg);
        
        RunServer(network_address, send_int, send_string, send_file);
    }    
    catch (std::exception const& e) 
    {
        std::cout << std::endl << "Exception: " << e.what() << std::endl;
        std::cout << param_message << std::endl;
        return 1;
    }

    return 0;
}
