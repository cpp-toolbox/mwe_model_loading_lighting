#include <GLFW/glfw3.h>

#include "mouse.hpp"

#include <tuple>


/**
 * \brief Get delta angles based on mouse
 *
 * \details Take in absolute mouse positions and turn them into delta angles wrt yaw and pitch
 *
 * \param mouse_position_x the current mouse x position
 * \param mouse_position_y the current mouse y position
 * \return a tuple t such that t1 is the change in yaw angle, and t2 is the change in pitch angle
 *
 * \author cuppajoeman
 * \date 2024
 */
std::tuple<double, double> Mouse::get_yaw_pitch_deltas(double mouse_position_x, double mouse_position_y) {

    double change_in_yaw_angle = this->sensitivity * (this->last_mouse_position_x - mouse_position_x);
    double change_in_pitch_angle = this->sensitivity * (this->last_mouse_position_y - mouse_position_y);

    std::tuple yaw_pitch_deltas = {change_in_yaw_angle, change_in_pitch_angle};

    this->last_mouse_position_x = mouse_position_x;
    this->last_mouse_position_y = mouse_position_y;

    return yaw_pitch_deltas;
}

void Mouse::configure_with_glfw(GLFWwindow *window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // tell GLFW to capture our mouse
}