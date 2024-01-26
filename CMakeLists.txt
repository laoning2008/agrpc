cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(agrpc CXX)

#CPM
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/AsioGrpcProtobufGenerator.cmake)

#grpc
set(ABSL_ENABLE_INSTALL ON)
CPMAddPackage(NAME GRPC GITHUB_REPOSITORY grpc/grpc GIT_TAG v1.60.0)   

#asio
CPMAddPackage(NAME ASIO GITHUB_REPOSITORY chriskohlhoff/asio GIT_TAG asio-1-28-2)
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${ASIO_SOURCE_DIR}/asio/include)             

#agrpc
add_compile_definitions(AGRPC_STANDALONE_ASIO)
CPMAddPackage(NAME AGRPC GITHUB_REPOSITORY Tradias/asio-grpc GIT_TAG v2.9.2 OPTIONS "ASIO_GRPC_USE_RECYCLING_ALLOCATOR ON")
# add_library(agrpc INTERFACE)
# target_include_directories(agrpc INTERFACE ${AGRPC_SOURCE_DIR}/src)

#examples
add_subdirectory(examples)


