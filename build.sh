LINK_PARAMS="-L/usr/local/lib -L/usr/local/lib -lprotobuf -pthread -lgrpc++ -lgrpc -lgrpc++_reflection -ldl"

protoc -I . --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` dataexchange.proto
protoc -I . --cpp_out=. dataexchange.proto

g++ *.cc main_client.cpp $LINK_PARAMS -o client_app
g++ *.cc main_server.cpp $LINK_PARAMS -o server_app

