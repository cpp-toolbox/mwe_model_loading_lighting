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
#include "graphics/render_primitives/divplo_collection.hpp"
#include "graphics/textured_model_loading/textured_model_loading.hpp"
#include "graphics/shader_cache/shader_cache.hpp"
#include "graphics/window/window.hpp"

#include "input/glfw_lambda_callback_manager/glfw_lambda_callback_manager.hpp"

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
                                              Character &character, DivplodtCollection &model,
                                              ShaderCache &shader_cache, GLFWwindow *window) {
    return [&live_input_state, &mouse, &camera, &character, &shader_cache, &model, window](double delta_time) {
        update_player_state(delta_time, live_input_state, mouse, camera, character);

        render(shader_cache, model, character,
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

    using CharCallback = std::function<void(unsigned int)>;
    using KeyCallback = std::function<void(int, int, int, int)>;
    using CursorPosCallback = std::function<void(double, double)>;
    using MouseButtonCallback = std::function<void(int, int, int)>;

    CharCallback on_char = [](int _){};
    KeyCallback on_key = [window, &live_input_state](int key, int scancode, int action, int mods) {
        // TODO figure out how to do mappings of key to function to simplify
        if (key == GLFW_KEY_Q) {
            if (action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        } else if (key == GLFW_KEY_F) {
            // TOGGLE FULLSCREEN
            // https://stackoverflow.com/a/47462358/6660685 <- implement this
        } else if (key == GLFW_KEY_A) {
            if (action == GLFW_PRESS) {
                live_input_state.left_pressed = true;
            } else if (action == GLFW_RELEASE) {
                live_input_state.left_pressed = false;
            }
        } else if (key == GLFW_KEY_D) {
            if (action == GLFW_PRESS) {
                live_input_state.right_pressed = true;
            } else if (action == GLFW_RELEASE) {
                live_input_state.right_pressed = false;
            }
        } else if (key == GLFW_KEY_W) {
            if (action == GLFW_PRESS) {
                live_input_state.forward_pressed = true;
            } else if (action == GLFW_RELEASE) {
                live_input_state.forward_pressed = false;
            }
        } else if (key == GLFW_KEY_S) {
            if (action == GLFW_PRESS) {
                live_input_state.backward_pressed = true;
            } else if (action == GLFW_RELEASE) {
                live_input_state.backward_pressed = false;
            }
        } else if (key == GLFW_KEY_SPACE) {
            if (action == GLFW_PRESS) {
                live_input_state.jump_pressed = true;
            } else if (action == GLFW_RELEASE) {
                live_input_state.jump_pressed = false;
            }
        } 
    };

    CursorPosCallback on_cursor = [window, &live_input_state] (double mouse_position_x, double mouse_position_y) {
        live_input_state.mouse_position_x = mouse_position_x;
        live_input_state.mouse_position_y = mouse_position_y;
    };

    MouseButtonCallback on_mouse_button =  [](int a, int b, int c) {};

    GLFWLambdaCallbackManager glcm(window, on_char, on_key, on_cursor, on_mouse_button);

    glEnable(GL_DEPTH_TEST); // configure global opengl state

    std::vector<ShaderType> requested_shaders = {ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES};

    ShaderCache shader_cache(requested_shaders);

    OpenGLTextureCache gl_texture_cache;

    TexturedModelLoader tmdldr;

    TexturedModel model = tmdldr.load_model("assets/backpack/backpack.obj");

    auto textured_model = DivplodtCollection(ShaderType::CWL_V_TRANSFORMATION_WITH_TEXTURES,
                                                                     model, shader_cache, gl_texture_cache);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw in wireframe

    FixedFrequencyLoop game_loop;

    std::function<int()> termination = [window]() { return glfwWindowShouldClose(window); };

    std::function<void(double)> game_step =
        game_step_closure(live_input_state, mouse, camera, character, textured_model, shader_cache, window);

    game_loop.start(tick_rate_hz, game_step, termination);

    glfwTerminate(); // glfw: terminate, clearing all previously allocated GLFW
                     // resources.
    return 0;
}
