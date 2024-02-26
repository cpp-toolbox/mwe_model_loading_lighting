#ifndef MWE_MODEL_LOADING_MOUSE_HPP
#define MWE_MODEL_LOADING_MOUSE_HPP

#include <tuple>

class Mouse {
    public:
        double sensitivity = 0.001;
        double last_mouse_position_x = 0.0, last_mouse_position_y = 0.0;
        std::tuple<double, double> get_yaw_pitch_deltas(double mouse_position_x, double mouse_position_y);
        void configure_with_glfw(GLFWwindow *window);
};

#endif //MWE_MODEL_LOADING_MOUSE_HPP
