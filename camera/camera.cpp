#include "camera.hpp"
#include "glm/ext/scalar_constants.hpp"
#include <cmath>

/**
 * \brief update's the camera's look direction based on changes changes in yaw and pitch
 * \author cuppajoeman
 * \date created: 2024, last edited: 2024-02-26
 */
void Camera::update_look_direction(double change_in_yaw_angle, double change_in_pitch_angle) {
    this->yaw_angle += change_in_yaw_angle;
    this->pitch_angle += change_in_pitch_angle;

    this->clamp_camera_pitch_angle();

    this->look_direction = Camera::yaw_pitch_angles_to_vector(this->yaw_angle, this->pitch_angle);
    this->look_direction = glm::normalize(this->look_direction);
}

/**
 * \brief converts a tuple (yaw, pitch), to a 3 dimensional vector pointing in that direction.
 * \bug converts doubles into floats causing less accurate aiming
 * \author cuppajoeman
 * \date created: 2024, last edited: 2024-02-26
 */
glm::vec3 Camera::yaw_pitch_angles_to_vector(double yaw, double pitch) {
    glm::vec3 vector;
    vector.x = cos(pitch) * sin(yaw);
    vector.y = sin(pitch);
    vector.z = cos(pitch) * cos(yaw);
    return vector;
}

/**
 * \brief clamp the mouse
 * \details clamps this camera's angle between -pi/2 to pi/2, with some margins
 * \note The margins are required because if we allow the camera to look straight up or down, opengl's view of the world gets
 * inverted.
 * \author cuppajoeman
 * \date created: 2023
 */
void Camera::clamp_camera_pitch_angle() {

    auto PI = glm::pi<float>();

    float angle_cutoff_margin = 0.001;

    float a_little_less_than_two_pi = PI/2 - angle_cutoff_margin;
    float a_little_more_than_negative_two_pi = -PI/2 + angle_cutoff_margin;

    if (this->pitch_angle > a_little_less_than_two_pi) {
        this->pitch_angle = a_little_less_than_two_pi;
    }

    if (this->pitch_angle < a_little_more_than_negative_two_pi) {
        this->pitch_angle = a_little_more_than_negative_two_pi;
    }
}