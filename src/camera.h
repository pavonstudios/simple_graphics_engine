#ifndef CAMERA_H
#define CAMERA_H

#ifndef ANDROID
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
#else
	#include "glm/glm.hpp"
	#include "glm/gtc/matrix_transform.hpp"
#endif

class Camera {
public:
	float screen_height = 600;
	float screen_width = 800;
	glm::vec3 cameraPos   = glm::vec3(0.0f, -25.0f,  0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraUp    = glm::vec3(0.0f, 0.0f,  1.0f);

	glm::mat4 Projection = glm::perspective(glm::radians(45.f), screen_width/screen_height, 0.01f, 5000.f);

	glm::mat4 View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	
	
	void update_projection_matrix(){
	Projection = glm::perspective(glm::radians(45.f), screen_width/screen_height, 0.01f, 5000.f);
	}
	void update(){
		View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	};		
	
	float cameraSpeed = 0.05f; 
	float velocity = 5.f;
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void mouse_control_update(float , float);
	void MoveUp();
	void MoveDown();
};

#endif
