#include "graphics.hpp"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void render(ShaderCache &shader_cache, DivplodtCollection &model, Character &character, Camera &camera,
            int screen_width, int screen_height) {
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto character_position = character.position;
    shader_cache.use_shader_program(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES);

    float fov = 90;
    float render_distance = 1200;
    glm::mat4 local_to_world = glm::mat4(1);
    glm::mat4 world_to_camera =
        glm::lookAt(character_position, character_position + camera.look_direction, camera.up_direction);
    glm::mat4 camera_to_clip =
        glm::perspective(glm::radians(fov), (float)screen_width / (float)screen_height, 0.1f, render_distance);

    shader_cache.set_uniform(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES, ShaderUniformVariable::LOCAL_TO_WORLD,
                             local_to_world);
    shader_cache.set_uniform(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES, ShaderUniformVariable::WORLD_TO_CAMERA,
                             world_to_camera);
    shader_cache.set_uniform(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES, ShaderUniformVariable::CAMERA_TO_CLIP,
                             camera_to_clip);

    model.draw();
}
