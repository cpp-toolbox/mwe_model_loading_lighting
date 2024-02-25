//
// Created by ccn on 25/02/24.
//

#ifndef MWE_MODEL_LOADING_CHARACTER_HPP
#define MWE_MODEL_LOADING_CHARACTER_HPP


#include "glm/vec3.hpp"
#include "../camera/camera.hpp"

class Character {
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    Camera camera;
};

#endif //MWE_MODEL_LOADING_CHARACTER_HPP
