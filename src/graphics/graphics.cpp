#include "graphics.hpp"
#include "shader_cache/shader_cache.hpp"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void render(ShaderCache &shader_cache, DivplodtnCollection &model, Character &character, Camera &camera,
            int screen_width, int screen_height) {
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto character_position = character.position;
    shader_cache.use_shader_program(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES_AMBIENT_AND_DIFFUSE_LIGHTING);

    float fov = 90;
    float render_distance = 1200;
    glm::mat4 local_to_world = glm::mat4(1);
    glm::mat4 world_to_camera =
        glm::lookAt(character_position, character_position + camera.look_direction, camera.up_direction);
    glm::mat4 camera_to_clip =
        glm::perspective(glm::radians(fov), (float)screen_width / (float)screen_height, 0.1f, render_distance);

    shader_cache.set_uniform(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES_AMBIENT_AND_DIFFUSE_LIGHTING, ShaderUniformVariable::LOCAL_TO_WORLD,
                             local_to_world);
    shader_cache.set_uniform(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES_AMBIENT_AND_DIFFUSE_LIGHTING, ShaderUniformVariable::WORLD_TO_CAMERA,
                             world_to_camera);
    shader_cache.set_uniform(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES_AMBIENT_AND_DIFFUSE_LIGHTING, ShaderUniformVariable::CAMERA_TO_CLIP,
                             camera_to_clip);

    float ambient_light_strength = 0.25;
    glm::vec3 ambient_light_color(1, 1, 0);
    glm::vec3 diffuse_light_position(0, 0, 0);
    shader_cache.set_uniform(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES_AMBIENT_AND_DIFFUSE_LIGHTING, ShaderUniformVariable::AMBIENT_LIGHT_STRENGTH, ambient_light_strength);
    shader_cache.set_uniform(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES_AMBIENT_AND_DIFFUSE_LIGHTING, ShaderUniformVariable::AMBIENT_LIGHT_COLOR, ambient_light_color);
    shader_cache.set_uniform(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES_AMBIENT_AND_DIFFUSE_LIGHTING, ShaderUniformVariable::DIFFUSE_LIGHT_POSITION, diffuse_light_position);

    model.draw();
}
