cmake_minimum_required(VERSION 3.0.0)
project(Game VERSION 0.1.0)

add_executable(Game src/main.cpp)

target_link_libraries(Game SDL2main SDL2 GL SDL2_ttf)
