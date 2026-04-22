cmake_minimum_required(VERSION 3.10)
project(StockExchange)
 
set(CMAKE_CXX_STANDARD 17)
 
include_directories(external src)
file(GLOB_RECURSE SOURCES src/*.cpp)
file(COPY web DESTINATION ${CMAKE_BINARY_DIR})
add_executable(stock_exchange ${SOURCES})