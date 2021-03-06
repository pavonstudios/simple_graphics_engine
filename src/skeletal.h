#ifndef SKELETAL_H
#define SKELETAL_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "tiny_class.h"
#include <glm/gtc/quaternion.hpp>

using namespace glm;

#define PATH_TYPE_NULL 0
#define PATH_TYPE_TRANSLATION 1
#define PATH_TYPE_ROTATION 2
class AnimationManager;

namespace engine{
	struct EMesh;
	struct Skin;

	struct Node{
		Node *parent;
		uint32_t index;
		glm::vec3 Translation{};
		glm::quat Rotation{};	
		std::vector<Node*> children;
		int32_t skin_index = -1;
		std::string name;   
		uint32_t bone_index;   
		glm::mat4 global_matrix = glm::mat4(1.0); 
		mat4 rot_mat = mat4(1.0); 
		
	};

	struct Skin {
		Node *skeleton_root = nullptr;
		std::vector<glm::mat4> inverse_bind_matrix;
		std::vector<Node*> joints;
	};
	
	struct NodeLoadData{
		tinygltf::Model* gltf_model;
		engine::Node *parent; 
		uint32_t index;
		tinygltf::Node *gltf_node;
	};

	struct AnimationChannel {
		uint8 PathType = PATH_TYPE_NULL; 
		Node* node;
		uint32 sampler_index;
		uint8 node_index;
	};

	struct AnimationSampler{
		std::vector<float> inputs;
		std::vector<glm::vec4> outputs_vec4;
	};

	struct Animation{
		float time = 0;
		bool loop = false;
		std::string name;
		std::vector<AnimationChannel> channels;
		std::vector<AnimationSampler> samplers;
		float start = std::numeric_limits<float>::max();
		float end = std::numeric_limits<float>::min();
	};

	struct NodeUniform{
        alignas(16) glm::mat4 matrix;
        alignas(16) glm::mat4 joint_matrix[200];
        float joint_count = 0;
    };
   
	struct SkeletalMesh{
		EMesh* mesh = nullptr;
		std::vector<Node*> nodes;
		std::vector<Node*> linear_nodes;
		Skin* skin = nullptr;
		struct NodeUniform node_uniform;
		std::vector<Animation*> animations;
		
	};
	

	
	class SkeletalLoader{
		public:
		SkeletalLoader();
		void load_skin(SkeletalMesh* mesh, tinygltf::Model &gltf_model);
	};

	class SkeletalManager{
	public:
		static void load_node(EMesh* mesh, NodeLoadData& node_data);
		static void load_node(SkeletalMesh* mesh, NodeLoadData& node_data);
		
		static void load_skin(EMesh* mesh, tinygltf::Model &gltf_model);		

		static void load_data(AnimationManager* manager, EMesh* mesh);
		
		static void update_joints_nodes(EMesh* mesh);		

		#ifdef DEVELOPMENT
		static void create_bones_vertices(Engine*);

		static void update_joint_vertices_data(Engine*);
		#endif

		static void joint_by_name(SkeletalMesh* , std::string);
	};

	class NodeManager{
		public:
			static void create_nodes_index(EMesh* mesh);
			static glm::mat4 get_local_matrix(Node* node);
			static glm::mat4 get_global_matrix(Node* node);
			static Node* node_by_name(EMesh* mesh, const char* name);
			static Node* node_from_index(EMesh* mesh, uint32_t index);

			static Node* find_node(Node* parent, uint32_t index);

			static Node* node_by_name(SkeletalMesh* mesh, const char* name);
			static Node* node_from_index(SkeletalMesh* mesh, uint32_t index);
			
	};

}
#endif