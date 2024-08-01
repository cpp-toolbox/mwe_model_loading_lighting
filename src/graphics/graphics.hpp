#ifndef MWE_MODEL_LOADING_GRAPHICS_HPP
#define MWE_MODEL_LOADING_GRAPHICS_HPP

#include "shader_pipeline/shader_pipeline.hpp"
#include "../graphics/render_primitives/div_collection.hpp"
#include "../interaction/character/character.hpp"

void render(GLuint shader_id, DiffuseTexturedDivCollection &model, Character &character, Camera &camera,
            int screen_width, int screen_height);

#endif // MWE_MODEL_LOADING_GRAPHICS_HPP