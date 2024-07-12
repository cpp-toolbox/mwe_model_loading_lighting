#ifndef MWE_MODEL_LOADING_GRAPHICS_HPP
#define MWE_MODEL_LOADING_GRAPHICS_HPP

#include "shader_pipeline/shader_pipeline.hpp"
#include "../graphics/model_loading/model_loading.hpp"
#include "../interaction/character/character.hpp"

void render(ShaderPipeline shader_pipeline, Model model, Character character, Camera camera, int screen_width, int screen_height);

#endif //MWE_MODEL_LOADING_GRAPHICS_HPP