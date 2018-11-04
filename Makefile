LDFLAGS=-L/usr/local/lib -L/usr/local/lib -lprotobuf -pthread -lgrpc++ -lgrpc -lgrpc++_reflection -ldl
CC=g++
CFLAGS=-O3

all:grpc client server

grpc:
	protoc -I . --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` dataexchange.proto
	protoc -I . --cpp_out=. dataexchange.proto

client:
	$(CC) *.cc $(CFLAGS) main_client.cpp $(LDFLAGS) -o client_app

server:
	$(CC) *.cc $(CFLAGS) main_server.cpp $(LDFLAGS) -o server_app

clean:
	rm client_app server_app -f
