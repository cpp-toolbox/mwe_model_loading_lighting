#include "graphics.hpp"

#include "shaders/CWL_uniform_binder_camera_pov.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void render(GLuint shader_id, Model &model, Character &character, Camera &camera, int screen_width, int screen_height) {
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    bind_CWL_matrix_uniforms_camera_pov(shader_id, screen_width, screen_height, character.position, glm::mat4(1.0f), camera, 90, 100);
    model.draw();
}
