#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "external_libraries/stb/stb_image.h"

#include <iostream>
#include "camera/camera.hpp"
#include "shader_pipeline/shader_pipeline.hpp"
#include "model_loading/model_loading.hpp"
#include "character/character.hpp"

float last_mouse_position_x = 0, last_mouse_position_y = 0;
float sensitivity = 0.001f;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Character character;

// camera
Camera camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double x_position, double y_position)
{
    // TODO variables names could be standardized
    float mouse_position_change_x = x_position - last_mouse_position_x;
    float mouse_position_change_y = y_position - last_mouse_position_y;

    // TODO figure out why we have to invert them...
    mouse_position_change_x = -mouse_position_change_x;
    mouse_position_change_y = -mouse_position_change_y;

    float change_in_yaw_angle = mouse_position_change_x * sensitivity;
    float change_in_pitch_angle = mouse_position_change_y * sensitivity;

    camera.update_look_direction(change_in_yaw_angle, change_in_pitch_angle);

    last_mouse_position_x = x_position;
    last_mouse_position_y = y_position;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 strafe_direction = glm::cross(camera.look_direction, camera.up_direction);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        character.position += camera.look_direction * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        character.position -= camera.look_direction * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        character.position += strafe_direction * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        character.position -= strafe_direction * deltaTime;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "mwe model loading", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
//    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    ShaderPipeline shader_pipeline;
    shader_pipeline.load_in_shaders_from_file("../shaders/matrix_transformation.vert", "../shaders/textured.frag");

    // load models
    // -----------
    Model model;
    model.load_model("../assets/backpack/backpack.obj");
    model.configure_vertex_interpretation_for_shader(shader_pipeline);


    // draw in wireframe
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        process_input(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        glUseProgram(shader_pipeline.shader_program_id);

        // view/projection transformations
        glm::mat4 camera_to_clip = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        GLint camera_to_clip_uniform_location = glGetUniformLocation(shader_pipeline.shader_program_id, "camera_to_clip");
        glUniformMatrix4fv(camera_to_clip_uniform_location , 1, GL_FALSE, glm::value_ptr(camera_to_clip));

        glm::mat4 world_to_camera = glm::lookAt(character.position, character.position + camera.look_direction, camera.up_direction);
        GLint world_to_camera_uniform_location = glGetUniformLocation(shader_pipeline.shader_program_id, "world_to_camera");
        glUniformMatrix4fv(world_to_camera_uniform_location, 1, GL_FALSE, glm::value_ptr(world_to_camera));
//        shader_pipeline.setMat4("view", view);

        // render the loaded model
        glm::mat4 local_to_world = glm::mat4(1.0f);
        local_to_world = glm::translate(local_to_world, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        local_to_world = glm::scale(local_to_world, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down


        GLint local_to_world_uniform_location = glGetUniformLocation(shader_pipeline.shader_program_id, "local_to_world");
        glUniformMatrix4fv(local_to_world_uniform_location , 1, GL_FALSE, glm::value_ptr(local_to_world));

        model.draw(shader_pipeline);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

