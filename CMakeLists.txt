cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mwe_model_loading)

file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the main executable
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(glfw3)
find_package(glad)
find_package(glm)
find_package(assimp)
find_package(stb)
find_package(spdlog)

target_link_libraries(${PROJECT_NAME} glfw glad::glad glm::glm assimp::assimp stb::stb spdlog::spdlog)