cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE "H:/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(grpc_example)

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)

add_executable( server src/server.cpp proto/helloworld.grpc.pb.cc proto/helloworld.pb.cc)
add_executable( client src/client.cpp proto/helloworld.grpc.pb.cc proto/helloworld.pb.cc)

target_link_libraries( server PRIVATE gRPC::grpc++ gRPC::grpc++_reflection gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite )
target_link_libraries( client PRIVATE gRPC::grpc++ gRPC::grpc++_reflection gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite )