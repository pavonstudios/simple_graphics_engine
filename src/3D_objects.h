#ifndef _3D_OBJ_H_
#define _3D_OBJ_H_


#define GLM_FORCE_RADIANS

#include <unordered_map>
#include <vector>
#include <array>

#ifndef ANDROID
    #ifndef _OpenGL_Renderer_
        #include <vulkan/vulkan.h>
    #else
        #include <GL/gl.h>
    #endif
#else
    #include <GLES2/gl2.h>
    #include "glm/glm.hpp"
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include "game_object.h"

#include "gltf.h"
#include "tiny_class.h"
#include "vertex.h"

#include "VulkanDevice.hpp"



namespace engine{
    struct Node;
    struct Skin {
        Node *skeleton_root = nullptr;
        std::vector<glm::mat4> inverse_bind_matrix;
        std::vector<Node*> joints;
    };
    
    struct NodeUniform{
        alignas(16) glm::mat4 matrix;
        alignas(16) glm::mat4 joint_matrix[128];
        float joint_count {0};
    };
   
}
struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

#ifndef ANDROID

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}
#endif


namespace engine{
class EMesh : public GameObject{
public:
#ifdef VULKAN
    VkDevice* pDevice;
    EMesh(vks::VulkanDevice* vulkan_device);
    vks::VulkanDevice* vulkan_device;
#else
    EMesh();
#endif
    ~EMesh();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector< glm::vec3 > simple_vertices;
    tinygltf::Model gltf_model;
    void load_model(std::string model_path);

    glm::mat4 model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec3 location_vector;
    std::string texture_path;

    std::vector<Node*> nodes;
    std::vector<Node*> linear_nodes;
    std::vector<Skin*> skins;

    //skeletal
    void load_joints_matrix();
    void load_node(engine::Node *parent, const tinygltf::Node &gltf_node);
    void load_skins();
    Node* node_from_index(uint32_t index);
    Node* find_node(Node* parent, uint32_t index);

    virtual void SetLocation(float x, float y, float z);

    bool load_model2(const char * path);
#ifdef GLTF
    int load_model_gltf(const char* path);
    
#endif

#ifdef VULKAN
    VkBuffer vertices_buffer;
    VkBuffer indexBuffer;
    VkBuffer node_unifrom_buffer;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<VkDescriptorSet> descriptorSets;
    VkDescriptorPool descriptorPool;
    VkPipeline graphics_pipeline;
    VkImage texture_image;
    VkImageView texture_image_view;

    std::vector<VkBuffer> uniform_node_buffers;
    std::vector<VkDeviceMemory> uniform_node_buffer_memory;
#endif
#ifdef _OpenGL_Renderer_
    GLuint vertexbuffer;
	GLuint VertexArrayID;
    GLuint indices_buffer;
#endif
    UniformBufferObject ubo;
    NodeUniform node_uniform = {glm::mat4(1.0),
                                {glm::mat4(1.0),glm::mat4(1.0),glm::mat4(1.0)},
                                2};
		
};
struct Node{
        Node *parent;
        uint32_t index;
        glm::vec3 Translation{};
        glm::quat Rotation{};
        glm::mat4 matrix;
        std::vector<Node*> children;
        EMesh* mesh;
        Skin* skin;
        int32_t skin_index = -1;
        glm::mat4 get_local_matrix(){
            return glm::translate(glm::mat4(1.0f),Translation) * glm::mat4(Rotation) * matrix;
        }
        glm::mat4 get_matrix(){
                glm::mat4 local_matrix = get_local_matrix();
                Node* parent = this->parent;
                while(parent){
                    local_matrix = parent->get_local_matrix() * local_matrix;
                    parent = parent->parent;
                }
                return local_matrix;
        }
        void update(){
            if(mesh){
                glm::mat4 matrix = get_matrix();
                if(skin){
                    mesh->node_uniform.matrix = matrix;

                    glm::mat4 inverse_transform = glm::inverse(matrix);
                    size_t joints_number = skin->joints.size();
                    mesh->node_uniform.joint_count = (float)joints_number;

                    for(size_t i = 0; i< skin->joints.size();i++){
                        Node* joint_node = skin->joints[i];
                        glm::mat4 joint_mat = joint_node->get_matrix() * skin->inverse_bind_matrix[i];
                        joint_mat = inverse_transform * joint_mat;
                        mesh->node_uniform.joint_matrix[i] = joint_mat;
                    }
                }
            }
            for(auto& child : children){
                child->update();
            }
        }
};
class SkeletalMesh{
	public:
		
};

}//namespace engine
#endif