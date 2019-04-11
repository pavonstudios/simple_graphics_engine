#include "engine.h"
#include "input_controller.h"


void Engine::main_loop(){
	 
	 while (!glfwWindowShouldClose(window)) {
		 	glfwPollEvents();
			update_input();
			get_time();
			main_camera.cameraSpeed = main_camera.velocity * deltaTime;
			app.main_loop();
			glfwSwapBuffers(window);
    }
	app.finish();
	glfwDestroyWindow(window);

    glfwTerminate();


}

void Engine::InitWindow(){
		move_y = 2;
		if( !glfwInit() )
		{
			fprintf( stderr, "Failed to initialize GLFW\n" );
			return;
		}
		
	#ifdef VULKAN
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	#endif

	#ifdef _OpenGL_Renderer_
			glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
			
	#endif

		
		window = glfwCreateWindow(800, 600, "Engine", nullptr, nullptr);
		if( window == NULL ){
			fprintf( stderr, "Failed to open GLFW window\n" );
			glfwTerminate();
			return;
		}
			glfwSetWindowUserPointer(window, this);

	#ifdef VULKAN	   
			glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	#endif
			glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
			glfwSetKeyCallback(window, key_callback);
			glfwSetCursorPosCallback(window, mouse_callback);
			glfwSetMouseButtonCallback(window,mouse_button_callback);
			//glfwSetScrollCallback(window,input.scroll_callback);

	#ifdef _OpenGL_Renderer_
			
			glfwMakeContextCurrent(window); // Initialize GLEW


			if (glewInit() != GLEW_OK) {
				fprintf(stderr, "Failed to initialize GLEW\n");
				return;
			}
	#endif
}

void Engine::update_window_size(){
	 int width = 0, height = 0;
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }
	main_camera.screen_width = width;
	main_camera.screen_height = height;
	main_camera.update_projection_matrix();
}

 void Engine::mouse_callback(GLFWwindow* window, double xpos, double ypos){
	#ifdef VULKAN
	  auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	#endif
	#ifdef _OpenGL_Renderer_
		auto app = reinterpret_cast<RendererGL*>(glfwGetWindowUserPointer(window));
	#endif

		if(!app){
			throw std::runtime_error("no app pointer");
		}
	
	if(app->engine->input.move_camera){
		if(app->engine->input.first_mouse){
			app->engine->input.lastX = xpos;
			app->engine->input.lastY = ypos;
			app->engine->input.first_mouse = false;
		}

		float xoffset = xpos - app->engine->input.lastX ;
		float yoffset = app->engine->input.lastY - ypos; // reversed since y-coordinates range from bottom to top
		app->engine->input.lastX = xpos;
		app->engine->input.lastY = ypos;

		float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		app->engine->input.yaw   += xoffset;
		app->engine->input.pitch += yoffset;  

		if(app->engine->input.pitch > 89.0f)
			app->engine->input.pitch =  89.0f;
		if(app->engine->input.pitch < -89.0f)
			app->engine->input.pitch = -89.0f;		

	}//end right click pressed
	
	

}

void Engine::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	#ifdef VULKAN
		auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	#endif
	#ifdef _OpenGL_Renderer_
		auto app = reinterpret_cast<RendererGL*>(glfwGetWindowUserPointer(window));
	#endif

		if (button == GLFW_MOUSE_BUTTON_RIGHT ){
			if(action == GLFW_PRESS){
				app->engine->input.right_button_pressed = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
				app->engine->input.move_camera = false;
			}
			if(action == GLFW_RELEASE){
				app->engine->input.right_button_pressed = false;
			}

		}
		if (button == GLFW_MOUSE_BUTTON_LEFT ){
			if(action == GLFW_PRESS){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
				app->engine->input.move_camera = true; 
			}
			if(action == GLFW_RELEASE){
				//app->engine->input.right_button_pressed = false;
			}

		}

}

void Engine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	#ifdef VULKAN
		auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	#endif
	#ifdef _OpenGL_Renderer_
		auto app = reinterpret_cast<RendererGL*>(glfwGetWindowUserPointer(window));
	#endif
          
			if(key == GLFW_KEY_S){
				if(action == GLFW_PRESS){
					app->engine->input.S.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					app->engine->input.S.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_W){
				if(action == GLFW_PRESS){
					app->engine->input.W.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					app->engine->input.W.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_D){
				if(action == GLFW_PRESS){
					app->engine->input.D.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					app->engine->input.D.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_A){
				if(action == GLFW_PRESS){
					app->engine->input.A.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					app->engine->input.A.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_Z){
				if(action == GLFW_PRESS){
					app->engine->input.Z.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					app->engine->input.Z.bIsPressed = false;
				}
			}
			
			if(key == GLFW_KEY_E){
				if(action == GLFW_PRESS){
					app->engine->input.E.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					app->engine->input.E.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_Q){
				if(action == GLFW_PRESS){
					app->engine->input.Q.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					app->engine->input.Q.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_X){
				if(action == GLFW_PRESS){
					app->engine->input.X.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					app->engine->input.X.bIsPressed = false;
				}
			}
           
           
}

void Engine::update_input(){
	if(input.S.bIsPressed){
		main_camera.MoveBackward();
		
	}
	if(input.W.bIsPressed){
		main_camera.MoveForward();
	}
	if(input.A.bIsPressed){		
		main_camera.MoveLeft();
	}
	if(input.D.bIsPressed){		
		main_camera.MoveRight();
		
	}
	if(input.Z.bIsPressed){
		meshes[1]->nodes[2]->move_root();
	}
	if(input.X.bIsPressed){	
		meshes[1]->nodes[2]->rotate();
	}

	if(input.Q.bIsPressed){
		main_camera.MoveDown();
	}
	if(input.E.bIsPressed){
		main_camera.MoveUp();
	}

	if(input.move_camera){
		main_camera.mouse_control_update(input.yaw, input.pitch);

	}
}

void Engine::load_models(){	
	//load gltf skinned mesh
	skeletal = new EMesh(vulkan_device);
	skeletal->load_model_gltf("models/cube2.gltf");
	skeletal->texture_path = "textures/character2.jpg";
	meshes.push_back(skeletal);
	glm::mat4 model_matrix = glm::mat4(1.0f);
	//model_matrix = glm::translate(model_matrix, glm::vec3(7,0,0));
	meshes.back()->model_matrix = model_matrix;
	

}

void Engine::load_and_instance_at_location(std::string path, glm::vec3 location){
	EMesh *model = new EMesh(vulkan_device);	
	model->load_model_gltf(path.c_str());
	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, location);
	model->model_matrix = model_matrix;
	meshes.push_back(model);
	//std::cout << "loading Emesh" << std::endl;

}

void Engine::load_map(std::string path){
	FILE* file = fopen(path.c_str(),"r");
	if(file == NULL){
		throw std::runtime_error("failed to load map file");

	}
	
	std::vector<std::string> models_paths;
	std::vector<glm::vec3> locations;
	std::vector<std::string> textures_paths;
	while(1){
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
        	break; 

		if(!(strcmp( lineHeader, "#" ) == 0)){
			if ( strcmp( lineHeader, "m" ) == 0 ){
				char model_path[256];
				glm::vec3 location;
				char texture_path[256];
				fscanf(file, "%s %f %f %f %s\n", model_path, &location.x, &location.y, &location.z, texture_path);
				models_paths.push_back(std::string(model_path));
				locations.push_back(location);
				textures_paths.push_back(std::string(texture_path));
			}
			
		}
		
	}
	
	for(uint i = 0; i < models_paths.size();i++){		
		load_and_instance_at_location(models_paths[i],locations[i]);
				
	}

	//add textures path
	for(uint i = 0; i < models_paths.size();i++){	
	meshes[i]->texture_path = textures_paths[i];
	}
	//load gltf model
	//load_models();
}

void Engine::delete_meshes(){
	for(auto mesh : meshes){
		delete mesh;
	}
}