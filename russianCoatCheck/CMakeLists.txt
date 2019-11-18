cmake_minimum_required (VERSION 3.0)

project (coatCheck)

add_executable(coatCheck main.cpp)
set_property(TARGET coatCheck PROPERTY CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

find_package(Catch2 CONFIG REQUIRED)

target_link_libraries(coatCheck PRIVATE Catch2::Catch2)