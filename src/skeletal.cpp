#include "engine.h"
#include "objects.h"

using namespace engine;
/*
Fill node uniform block of joints matrix for send to the vertex shader
*/
void SkeletalManager::update_joints_nodes(EMesh* mesh){
    
    Skin* skin = mesh->skins[0];
    size_t joints_number = skin->joints.size();
    mesh->node_uniform.joint_count = (float)joints_number;  

    for(size_t i = 0; i < skin->joints.size(); i++){

        Node* joint = skin->joints[i];
     
        mat4 local = NodeManager::get_global_matrix(joint);
        local = mesh->model_matrix * local;

        glm::mat4 joint_mat =

            inverse(mesh->model_matrix) * 
            local * 
            skin->inverse_bind_matrix[i];


        mesh->node_uniform.joint_matrix[i] = joint_mat;
        
    } 

}

SkeletalLoader::SkeletalLoader(){

}
void SkeletalManager::load_data(AnimationManager* manager, EMesh* mesh){
    SkeletalMesh* skel_mesh = new SkeletalMesh;
    skel_mesh->mesh = mesh;
    mesh->skeletal = skel_mesh;
    
    int node_count = mesh->gltf_model.nodes.size();
    for(int i = 0; i < node_count;i++){
        NodeLoadData load_data = {};
        load_data.gltf_model = &mesh->gltf_model;
        load_data.gltf_node = &mesh->gltf_model.nodes[i];
        load_data.index = i;
        load_data.parent = nullptr;
        SkeletalManager::load_node(mesh,load_data);
        SkeletalManager::load_node(skel_mesh,load_data);        
    }
  
    SkeletalManager::load_skin(mesh, mesh->gltf_model);
    manager->skeletal_loader->load_skin(skel_mesh,mesh->gltf_model);

    NodeManager::create_nodes_index(mesh);//bones index numeration 

    manager->load_animation(skel_mesh,mesh->gltf_model);

    SkeletalManager::update_joints_nodes(mesh);
}

void NodeManager::create_nodes_index(EMesh* mesh){
    int index = 0;
    Skin* skin = mesh->skins[0];
    int joint_count = skin->joints.size();
    for(int i = 0;i<joint_count;i++){
        Node* joint_node = skin->joints[i];
        joint_node->bone_index = index;
        index++;
    }
}

glm::mat4 NodeManager::get_local_matrix(Node* node){
    glm::mat4 local = glm::translate(glm::mat4(1.0f),node->Translation) * glm::mat4(node->Rotation) * node->rot_mat;
    return local;
}

glm::mat4 NodeManager::get_global_matrix(Node* node){
    glm::mat4 local_matrix = get_local_matrix(node);
    Node* node_parent = node->parent;
    while(node_parent){        
        local_matrix = get_local_matrix(node_parent) * local_matrix;
        node_parent = node_parent->parent;
    }
    return local_matrix;

}
void SkeletalLoader::load_skin(SkeletalMesh* mesh, tinygltf::Model &gltf_model){
    for(tinygltf::Skin &source_skin: gltf_model.skins){
        Skin *new_skin = new Skin{};
        if(source_skin.skeleton > -1){
            new_skin->skeleton_root = NodeManager::node_from_index(mesh, source_skin.skeleton);
        }
        for(int joint_index : source_skin.joints){
            Node* node = NodeManager::node_from_index(mesh, joint_index);
            if(node)
                new_skin->joints.push_back(node);
        }
        if(source_skin.inverseBindMatrices > -1){
            const tinygltf::Accessor &accessor = gltf_model.accessors[source_skin.inverseBindMatrices];

            const tinygltf::BufferView &bufferView = gltf_model.bufferViews[accessor.bufferView];
            
            const tinygltf::Buffer &buffer = gltf_model.buffers[bufferView.buffer];
            
            new_skin->inverse_bind_matrix.resize(accessor.count);
            
            memcpy(new_skin->inverse_bind_matrix.data(),
                     &buffer.data[accessor.byteOffset + bufferView.byteOffset],
                     accessor.count * sizeof(glm::mat4));

        }

        //mesh->skins.push_back(new_skin);
        mesh->skin = new_skin;
    }
    
    
}
void SkeletalManager::load_skin(EMesh* mesh, tinygltf::Model &gltf_model){
    for(tinygltf::Skin &source_skin: gltf_model.skins){
        Skin *new_skin = new Skin{};
        if(source_skin.skeleton > -1){
            new_skin->skeleton_root = NodeManager::node_from_index(mesh, source_skin.skeleton);
        }
        for(int joint_index : source_skin.joints){
            Node* node = NodeManager::node_from_index(mesh, joint_index);
            if(node)
                new_skin->joints.push_back(node);
        }
        if(source_skin.inverseBindMatrices > -1){
            const tinygltf::Accessor &accessor = gltf_model.accessors[source_skin.inverseBindMatrices];

            const tinygltf::BufferView &bufferView = gltf_model.bufferViews[accessor.bufferView];
            
            const tinygltf::Buffer &buffer = gltf_model.buffers[bufferView.buffer];
            
            new_skin->inverse_bind_matrix.resize(accessor.count);
            
            memcpy(new_skin->inverse_bind_matrix.data(),
                     &buffer.data[accessor.byteOffset + bufferView.byteOffset],
                     accessor.count * sizeof(glm::mat4));

        }

        mesh->skins.push_back(new_skin);
    }
    
}

Node* NodeManager::find_node(Node* parent, uint32_t index){
    Node* node_found = nullptr;
    if(parent->index == index)
        return parent;
    for(auto& child : parent->children){
        node_found = find_node(child,index);
        if(node_found)
            break;
    }
    return node_found;
}
Node* NodeManager::node_from_index(SkeletalMesh* mesh, uint32_t index){
    Node* node_found = nullptr;
    for(auto &node : mesh->nodes){
        node_found = find_node(node,index);
        if(node_found)
            break;
    }
    return node_found;
}

Node* NodeManager::node_by_name(SkeletalMesh* mesh, const char* name ){
    Node* node_found = nullptr;
    for(auto node : mesh->nodes){
        if(node->name == name){
            node_found = node;
        }        
        if(node_found)
            break;
    }
	if (!node_found) {
		//std::cout << "node not found : " << std::string(name) << std::endl;
	}
       
    return node_found;
}

Node* NodeManager::node_from_index(EMesh* mesh, uint32_t index){
    Node* node_found = nullptr;
    for(auto &node : mesh->nodes){
        node_found = find_node(node,index);
        if(node_found)
            break;
    }
    return node_found;
}

Node* NodeManager::node_by_name(EMesh* mesh, const char* name ){
    Node* node_found = nullptr;
    for(auto node : mesh->nodes){
        if(node->name == name){
            node_found = node;
        }        
        if(node_found)
            break;
    }
	if (!node_found) {
		//std::cout << "node not found : " << std::string(name) << std::endl;
	}
      
    return node_found;
}

void SkeletalManager::load_node(EMesh* mesh, NodeLoadData& node_data){
     
    Node *new_node = new Node{};
    new_node->parent = node_data.parent;
    new_node->skin_index = node_data.gltf_node->skin;
    new_node->index = node_data.index;
    new_node->name = node_data.gltf_node->name;
    
    //some nodes do not contain transform information
    if(node_data.gltf_node->translation.size() == 3)
        new_node->Translation = glm::make_vec3(node_data.gltf_node->translation.data());

    if(node_data.gltf_node->rotation.size() == 4)
        new_node->Rotation = glm::make_quat(node_data.gltf_node->rotation.data());


    int children_count = node_data.gltf_node->children.size();

    if( children_count > 0){
        for(int i = 0;i < children_count ;i++){
            mesh->nodes[node_data.gltf_node->children[i]]->parent = new_node;
        }
        
    }

    if(node_data.parent){
        node_data.parent->children.push_back(new_node);       
    }else{
         mesh->nodes.push_back(new_node);
    }

    mesh->linear_nodes.push_back(new_node);
}

void SkeletalManager::load_node(SkeletalMesh* mesh, NodeLoadData& node_data){
     
    Node *new_node = new Node{};
    new_node->parent = node_data.parent;       
    new_node->index = node_data.index;
    new_node->name = node_data.gltf_node->name;
    
    //some nodes do not contain transform information
    if(node_data.gltf_node->translation.size() == 3)
        new_node->Translation = glm::make_vec3(node_data.gltf_node->translation.data());

    if(node_data.gltf_node->rotation.size() == 4)
        new_node->Rotation = glm::make_quat(node_data.gltf_node->rotation.data());

    int children_count = node_data.gltf_node->children.size();

    if( children_count > 0){
        for(int i = 0;i < children_count ;i++){
            mesh->nodes[ node_data.gltf_node->children[i] ]->parent = new_node;
        }        
    }
    
    if(node_data.parent){
        node_data.parent->children.push_back(new_node);       
    }else{
        mesh->nodes.push_back(new_node);
    }

    mesh->linear_nodes.push_back(new_node);
}

#ifdef DEVELOPMENT
void SkeletalManager::update_joint_vertices_data(Engine* engine){
    EMesh* mesh = engine->helpers[0];
    mesh->vertices.clear();

    for(auto* node : engine->skeletal_meshes[0]->skins[0]->joints){
        mat4 local = NodeManager::get_global_matrix(node);
        local = engine->skeletal_meshes[0]->model_matrix * local;
        Vertex vert {};
        vec3 position =  vec3(local[3]);
        vert.pos = position;
        mesh->vertices.push_back(vert);
    }
   
    #if defined(LINUX) && defined(ES2)
    glBindBuffer(GL_ARRAY_BUFFER,mesh->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER,mesh->vertices.size() * sizeof(Vertex),mesh->vertices.data(),GL_STATIC_DRAW);
    #endif
}

void SkeletalManager::create_bones_vertices(Engine* engine){
   
    EMesh* triangle = new EMesh();
    Skin* skin = engine->skeletal_meshes[0]->skins[0];

    for( size_t i = 0; i < skin->joints.size() ; i++ ){
        mat4 local = NodeManager::get_global_matrix(skin->joints[i]);
        local = engine->skeletal_meshes[0]->model_matrix * local;
        Vertex vert {};
        vec3 position =  vec3(local[3]);
        vert.pos = position;
        triangle->vertices.push_back(vert);
        
        if(skin->joints[i]->parent){
            
            if(i == 2){
                triangle->indices.push_back(i-1);
            }else if (i >= 3){
                triangle->indices.push_back(skin->joints[i]->parent->bone_index);
            }

        }
        triangle->indices.push_back(i);

    }

    
    triangle->data_shader.fragment_shader_path = "Game/Assets/shaders/gles/blue.glsl";

    
    triangle->data_shader.vertex_shader_path = "Game/Assets/shaders/gles/vert_mvp.glsl";
   
    triangle->type = -1;
    triangle->bIsGUI = false;
    triangle->name = "joints";   
    

    #if defined (ES2) || defined (ANDROID) 
        engine->linear_meshes.push_back(triangle);
        engine->helpers.push_back(triangle);        
    #endif
}
#endif

void SkeletalManager::joint_by_name(SkeletalMesh* mesh, std::string name ){
    for(Node* joint : mesh->nodes){

    }
}