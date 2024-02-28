#ifndef MODEL_LOADING_HPP
#define MODEL_LOADING_HPP

#include <string>
#include <vector>
#include "glm/glm.hpp"

#include "assimp/scene.h"
#include "../shader_pipeline/shader_pipeline.hpp"


/**
 * description:
 * 	a vertex in the context of a 3d model
 */
struct Vertex {
    glm::vec3 position;
    // A vertex doesn't really have a normal, but if we consider it
    // as part of a face, then it does, and could have many by
    // specifying it more than once in the input vertex list
    glm::vec3 normal;
    glm::vec2 texture_coordinate;
};

/**
 * description:
 * 	a texture along with a unique identifier having either specular or diffuse type
 */
struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

/**
 * description:
 * 	a collection of vertices along with an ordering of how to traverse along with a texture for the mesh
 *
 * notes:
 * 	 this mesh does not incorporate lighting and we will only apply one texture
 *
 * example:
 * 	a simple hamburger 3d model might have 3 meshes, one for the bottom bun, the burger and the top bun
 *
 */
class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(ShaderPipeline &shader_pipeline);
    void configure_vertex_interpretation_for_shader(ShaderPipeline &shader_pipeline);
private:
    void bind_vertex_data_to_opengl_for_later_use();
    void bind_vertex_attribute_interpretation_to_opengl_for_later_use(ShaderPipeline &shader_pipeline);
    unsigned int vertex_attribute_object{}, vertex_buffer_object{}, element_buffer_object{};
    void setup_mesh(ShaderPipeline &shader_pipeline);
};

/**
 * todo:
 * 	this is not really a model, it's a thing that creates a model...
 * 	this is a different kind of object then a mesh in a way
 * 	one way to fix this is to make a true model class and then make this a model_creator class or something like that
 *
 * description:
 * 	a collection of meshes, provides the ability to load from file and to draw it
 *
 * example:
 * 	a 3d model that represents the entire burger as in the mesh example
 */
class Model {
public:
    // Should the shader be stored inside of the model class? Unique to each shader?
    void load_model(std::string path);
    void draw(ShaderPipeline &shader_pipeline);
    void configure_vertex_interpretation_for_shader(ShaderPipeline &shader_pipeline);
    std::vector<Mesh> meshes;
private:
    std::string directory;

    std::vector<Texture> already_loaded_textures;

    void process_node(aiNode *node, const aiScene *scene);

    std::vector<Vertex> process_mesh_vertices(aiMesh *mesh);
    std::vector<unsigned int> process_mesh_indices(aiMesh *mesh);
    std::vector<Texture> process_mesh_materials(aiMesh *mesh, const aiScene *scene);
    Mesh process_mesh(aiMesh *mesh, const aiScene *scene);

    std::tuple<bool, int> texture_already_loaded(aiString texture_path);
    std::vector<Texture> load_material_textures(aiMaterial *material, aiTextureType texture_type, std::string type_name);
};

#endif //MODEL_LOADING_HPP