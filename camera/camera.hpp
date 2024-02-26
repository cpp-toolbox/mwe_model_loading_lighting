#ifndef MWE_MODEL_LOADING_CAMERA_HPP
#define MWE_MODEL_LOADING_CAMERA_HPP

#include "glm/glm.hpp"

class Camera {
public:
    // measured in radians
    double yaw_angle = 0.0;
    double pitch_angle = 0.0;
    glm::vec3 look_direction = glm::vec3(0.0, 0.0, 1.0);
    glm::vec3 up_direction = glm::vec3(0.0, 1.0, 0.0);
    void update_look_direction(double change_in_yaw_angle, double change_in_pitch_angle);
private:
    static glm::vec3 yaw_pitch_angles_to_vector(double yaw, double pitch);
    void clamp_camera_pitch_angle();
};

#endif //MWE_MODEL_LOADING_CAMERA_HPP
