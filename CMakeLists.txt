cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mwe_model_loading)

add_executable(mwe_model_loading
        src/main.cpp

        src/interaction/camera/camera.cpp
        src/interaction/mouse/mouse.cpp

        src/graphics/textured_model_loading/textured_model_loading.cpp
        src/graphics/shader_pipeline/shader_pipeline.cpp
        src/graphics/window/window.cpp
        src/graphics/graphics.cpp

        src/utils/fixed_frequency_loop/fixed_frequency_loop.cpp
        src/utils/fixed_frequency_loop/stopwatch/stopwatch.cpp

        src/graphics/shaders/CWL_uniform_binder_camera_pov.cpp
)

find_package(glfw3)
find_package(glad)
find_package(glm)
find_package(assimp)
find_package(stb)

target_link_libraries(${PROJECT_NAME} glfw glad::glad glm::glm assimp::assimp stb::stb)