#ifndef GAMEMAPS
#define GAMEMAPS
class Engine;

#include <sstream>
#include <iostream>
#include <fstream>

#include "objects.h"
using namespace engine;

struct Map{

};

class MapManager{
public:
	Engine * engine;
	void load(std::string path);
	void create_meshes_with_map_loaded_data();
	void create_mesh_with_data(MapDataToLoad &data);
	void parse_map_file(std::stringstream &file);

	void assign_shader_path();
	void load_meshes_for_instance(struct MapDataToLoad &data);
	void load_skeletal_meshes();

	std::vector<std::string> same_textures;
	std::vector<std::string> meshes_paths;
	std::vector<MapDataToLoad> meshes_load_data;
	std::vector<MapDataToLoad> unique_model_data;

	std::vector<int> skeletal_id;	
};
#endif