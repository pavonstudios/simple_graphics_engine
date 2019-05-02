#include "game_map.hpp"
#include "engine.h"
#include "Game/game.hpp"


void MapManager::load_data_from_file(std::stringstream &file){


		std::string line;
		int counter = 0;
		std::vector<int> skeletal_id;
		int type = 0;

		

		std::vector<load_data> meshes_load_data;
		
		

		while( std::getline(file,line) ) {		

			if(line != ""){
				char first_char;
				std::stringstream line_stream (line);
			
				std::string model_path;
				glm::vec3 location;
				std::string texture_path;
				std::string type;
				int mesh_type = 0;

				line_stream >> first_char;			
				if(first_char == '/'){
					break;
				}
				if(first_char == 'v'){
					line_stream >> texture_path;
					same_textures.push_back(texture_path);
				}
				if(first_char != '#' && first_char != 'v'){
					load_data data = {};

					line_stream >> model_path >> location.x >> location.y >> location.z >> texture_path >> type;
					if(type == "LOD"){
						mesh_type = MESH_LOD;
					}
					if(first_char == 'c'){
						texture_path = "textures/car01.jpg";
					}
					if(first_char == 's'){
						skeletal_id.push_back(counter);
					}
					if(first_char == 'a'){
						//with collider
						type = MESH_WITH_COLLIDER;
					}
									
					if(engine->game->player_id == -1){
						if(type == "player"){
							engine->game->player_id = counter;
							
						}
					}

					if(first_char == 'b'){
						texture_path = same_textures[0];
						data.texture_id = 0;
					}else{
						data.texture_id = -1;
					}
					
					counter++;

					
					data.model_path = model_path;
					data.texture_path = texture_path;
					data.location = location;
					data.type = mesh_type;

					meshes_load_data.push_back(data);
					
				}			
			
			}		
			
		
		}

		for(auto &data : meshes_load_data){
			create_mesh_with_data(data);
		}
		#ifdef VULKAN

			pipeline_data data_skinned_mesh = {};
		
			data_skinned_mesh.draw_type = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			
			data_skinned_mesh.mesh_type = MESH_TYPE_SKINNED;
			data_skinned_mesh.fragment_shader_path = "Game/Assets/shaders/frag.spv";
			data_skinned_mesh.vertex_shader_path = "Game/Assets/shaders/skin.spv";

			for(int id : skeletal_id){//assing skinned shader
				engine->linear_meshes[id]->data = data_skinned_mesh;
				Skeletal::load_data(engine->linear_meshes[id]);
			}
		#endif
}

void MapManager::create_mesh_with_data(struct load_data data){
	std::string path = engine->assets.path(data.model_path);
	vec3 location = data.location;

	#ifdef VULKAN
		EMesh *model = new EMesh(engine->vulkan_device);//vulkan device for create vertex buffers
		model->texture.format = VK_FORMAT_R8G8B8A8_UNORM;			
	#else
		EMesh *model = new EMesh();
    #endif
	#ifdef ANDROID
		engine->mesh_manager.load_mode_gltf_android(model,path.c_str(),engine->pAndroid_app->activity->assetManager);
	#else
		engine->mesh_manager.load_model_gltf(model, path.c_str());
	#endif

	model->name = data.model_path;

	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, location);
	model->location_vector = location;
	model->model_matrix = model_matrix;
	if(data.type != MESH_LOD){
		engine->meshes.push_back(model);
		
	}
	model->texture_path = engine->assets.path(data.texture_path);
	model->texture.texture_id = data.texture_id;
	engine->linear_meshes.push_back(model);
}