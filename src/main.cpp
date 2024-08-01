#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include <optional>
#include <iostream>

#include "interaction/camera/camera.hpp"
#include "interaction/character/character.hpp"
#include "interaction/mouse/mouse.hpp"

#include "graphics/graphics.hpp"
#include "graphics/render_primitives/div_collection.hpp"
#include "graphics/textured_model_loading/textured_model_loading.hpp"
#include "graphics/shader_pipeline/shader_pipeline.hpp"
#include "graphics/shader_cache/shader_cache.hpp"
#include "graphics/window/window.hpp"

#include "utils/fixed_frequency_loop/fixed_frequency_loop.hpp"
#include "utils/spdlog_setup/spdlog_setup.hpp"
#include "utils/client_systems_namespace/systems_namespace.hpp"
#include "utils/opengl_texture_cache/opengl_texture_cache.hpp"

unsigned int SCR_WIDTH = 1000;
unsigned int SCR_HEIGHT = 1000;
int tick_rate_hz = 60;

void update_player_state(double delta_time, LiveInputState &live_input_state, Mouse &mouse, Camera &camera,
                         Character &character) {
    auto [change_in_yaw_angle, change_in_pitch_angle] =
        mouse.get_yaw_pitch_deltas(live_input_state.mouse_position_x, live_input_state.mouse_position_y);
    camera.update_look_direction(change_in_yaw_angle, change_in_pitch_angle);

    glm::vec3 strafe_direction = glm::cross(camera.look_direction, camera.up_direction);

    auto float_delta_time = (float)delta_time;

    if (live_input_state.forward_pressed)
        character.position += camera.look_direction * float_delta_time;
    if (live_input_state.backward_pressed)
        character.position -= camera.look_direction * float_delta_time;
    if (live_input_state.left_pressed)
        character.position -= strafe_direction * float_delta_time;
    if (live_input_state.right_pressed)
        character.position += strafe_direction * float_delta_time;
}

std::function<void(double)> game_step_closure(LiveInputState &live_input_state, Mouse &mouse, Camera &camera,
                                              Character &character, DiffuseTexturedDivCollection &model,
                                              ShaderCache &shader_cache, GLFWwindow *window) {
    return [&live_input_state, &mouse, &camera, &character, &shader_cache, &model, window](double delta_time) {
        update_player_state(delta_time, live_input_state, mouse, camera, character);

        render(shader_cache.get_shader_program(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES).id, model, character,
               camera, SCR_WIDTH, SCR_HEIGHT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    };
}

int main() {

    std::vector<std::string> used_systems = {Systems::all};
    spdlog_setup(used_systems);

    Character character;
    Camera camera;
    Mouse mouse;
    bool start_in_fullscreen = false;
    bool capture_mouse = true;

    LiveInputState live_input_state;
    GLFWwindow *window = initialize_glfw_glad_and_return_window(
        &SCR_HEIGHT, &SCR_HEIGHT, "mwe model loading", start_in_fullscreen, capture_mouse, false, &live_input_state);

    glEnable(GL_DEPTH_TEST); // configure global opengl state

    std::vector<ShaderType> requested_shaders = {ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES};

    ShaderCache shader_cache(requested_shaders);

    OpenGLTextureCache gl_texture_cache;

    TexturedModelLoader tmdldr;
    TexturedModel model = tmdldr.load_model("../../assets/backpack/backpack.obj");

    DiffuseTexturedDivCollection dtdc = DiffuseTexturedDivCollection(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES,
                                                                     model, shader_cache, gl_texture_cache);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw in wireframe

    FixedFrequencyLoop game_loop;

    std::function<int()> termination = [window]() { return glfwWindowShouldClose(window); };

    std::function<void(double)> game_step =
        game_step_closure(live_input_state, mouse, camera, character, dtdc, shader_cache, window);

    game_loop.start(tick_rate_hz, game_step, termination);

    glfwTerminate(); // glfw: terminate, clearing all previously allocated GLFW
                     // resources.
    return 0;
}
