cmake_minimum_required(VERSION 3.1)

set(SOURCE_EXE main.cpp)

set(SOURCE_LIB Order.cpp OrderBook.cpp)

add_library(orders STATIC ${SOURCE_LIB})

add_executable(main ${SOURCE_EXE})

target_link_libraries(main orders)	