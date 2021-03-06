#include "camera.h"
void Camera::MoveForward(){
	 cameraPos += cameraSpeed * cameraFront;
	 update_look_at();
}

void Camera::MoveBackward(){
	cameraPos -= cameraSpeed * cameraFront;
	update_look_at();
}

void Camera::MoveLeft(){
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	update_look_at();
	
}

void Camera::MoveRight(){
	
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


	update_look_at();

}
void Camera::mouse_control_update(float yaw, float pitch){
	glm::vec3 front;
#ifdef VULKAN
    front.x = cos(glm::radians(-yaw)) * cos(glm::radians(pitch));
    front.z = sin(glm::radians(pitch));
    front.y = sin(glm::radians(-yaw)) * cos(glm::radians(pitch));
    
#else
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
#endif
	cameraFront = glm::normalize(front);
	//cameraPos = glm::normalize(glm::cross(cameraFront, cameraUp));

	update_look_at();
}

void Camera::MoveDown(){
	cameraPos -= cameraSpeed * cameraUp;
	update_look_at();
}

void Camera::update_look_at()
{
#ifndef DX11
	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
#else
	View = glm::lookAtLH(cameraPos, cameraPos + cameraFront, cameraUp);
#endif // !DX11

	
}

void Camera::MoveUp(){
	cameraPos += cameraSpeed * cameraUp;
	update_look_at();
}

void Camera::update_projection_matrix() {
#ifndef DX11
	Projection = glm::perspective(glm::radians(45.f), screen_width / screen_height, 0.01f, 5000.f);
#else
	Projection = glm::perspectiveLH(glm::radians(45.f), screen_width / screen_height, 0.01f, 5000.f);
	Projection = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.5f)) * glm::scale(glm::mat4(1.0), glm::vec3(1.0f, 1.0f, 0.5f)) * Projection;
#endif
}

void Camera::update() {
	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
};

Camera::Camera() {
	update_projection_matrix();
	update_look_at();
}
