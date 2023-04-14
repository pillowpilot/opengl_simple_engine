cmake_minimum_required(VERSION 3.15)
project(simple_engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # To generate /build/compile_commands.json and use it to feed VSCode IntelliSense.

find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
find_package(cereal REQUIRED)
find_package(assimp REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/cube.cpp src/model.cpp src/mesh.cpp src/utils.cpp src/shader.cpp src/vertex_shader.cpp src/fragment_shader.cpp src/program.cpp)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} cereal::cereal)
target_link_libraries(${PROJECT_NAME} assimp::assimp)

install(TARGETS ${PROJECT_NAME} DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
