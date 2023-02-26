cmake_minimum_required(VERSION 3.15)
project(simple_engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # To generate /build/compile_commands.json and use it to feed VSCode IntelliSense.

find_package(ZLIB REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(spdlog REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ZLIB::ZLIB)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
