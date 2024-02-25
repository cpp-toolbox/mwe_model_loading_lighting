#ifndef MWE_MODEL_LOADING_CAMERA_HPP
#define MWE_MODEL_LOADING_CAMERA_HPP

#include "glm/glm.hpp"

class Camera {
public:
    // measured in radians
    float yaw_angle = 0.0f;
    float pitch_angle = 0.0f;
    glm::vec3 look_direction = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 up_direction = glm::vec3(0.0f, 1.0f, 0.0f);
    void update_look_direction(float change_in_yaw_angle, float change_in_pitch_angle);
private:
    static glm::vec3 yaw_pitch_angles_to_vector(float yaw, float pitch);
    void clamp_camera_pitch_angle();
};

#endif //MWE_MODEL_LOADING_CAMERA_HPP
