cmake_minimum_required(VERSION 3.10)

project(mwe_model_loading)

add_executable(mwe_model_loading main.cpp
        camera/camera.cpp
        model_loading/model_loading.cpp
        shader_pipeline/shader_pipeline.cpp
)

# GLAD: opengl function loader

include_directories(external_libraries/glad_opengl_3.3_core/include)
add_subdirectory(external_libraries/glad_opengl_3.3_core)

# GLM: opengl mathematics

include_directories(external_libraries/glm)
add_subdirectory(external_libraries/glm)

# GLFW: opengl framework

# disable unnessary steps in build
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# ASSIMP: Asset Importer

# only use obj to speed up compilation
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)

include_directories(external_libraries/assimp/include)
add_subdirectory(external_libraries/assimp)

add_subdirectory(external_libraries/glfw)

target_link_libraries(mwe_model_loading glfw glad assimp)