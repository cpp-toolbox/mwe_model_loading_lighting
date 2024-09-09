#ifndef MWE_MODEL_LOADING_GRAPHICS_HPP
#define MWE_MODEL_LOADING_GRAPHICS_HPP

#include "../interaction/character/character.hpp"
#include "render_primitives/divplo_collection.hpp"

void render(ShaderCache &shader_cache, DivplodtnCollection &model, Character &character, Camera &camera, int screen_width, int screen_height);

#endif // MWE_MODEL_LOADING_GRAPHICS_HPP
