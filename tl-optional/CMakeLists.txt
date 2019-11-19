cmake_minimum_required (VERSION 3.0)

project (optional)

add_executable(optional main.cpp)
set_property(TARGET optional PROPERTY CXX_STANDARD 17)

find_package(Catch2 CONFIG REQUIRED)
find_package(tl-optional CONFIG REQUIRED)

target_link_libraries(optional PRIVATE Catch2::Catch2)
target_link_libraries(optional PRIVATE tl::optional)