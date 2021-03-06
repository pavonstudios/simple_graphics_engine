#include "engine.h"
#include "input.h"
#include "objects.h"
#include "camera.h"

#ifdef WINDOWS
void Input::handle_input_windows_messages(WPARAM wParam)
{
	if (wParam == 0x41) {
		std::cout << "a pressed \n";
	}
}

void Input::handle_key_released(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_TAB:
		this->TAB.Released = true;
		break;
	case KEY_A:
		this->A.bIsPressed = false;
		break;
	case KEY_D:
		this->D.bIsPressed = false;
		break;
	case KEY_S:
		this->S.bIsPressed = false;
		break;
	case KEY_W:
		this->W.bIsPressed = false;
		break;
	case KEY_Q:
		this->Q.bIsPressed = false;
		break;
	case KEY_E:
		this->E.bIsPressed = false;
		break;
	case VK_SPACE:
		this->SPACE.bIsPressed = false;
		break;
	default:
		break;
	}
}

void Input::handle_key_pressed(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_TAB:
		this->TAB.bIsPressed = true;
		break;
	case KEY_A:
		this->A.bIsPressed = true;
		break;
	case KEY_D:
		this->D.bIsPressed = true;
		break;
	case KEY_S:
		this->S.bIsPressed = true;
		break;
	case KEY_W:
		this->W.bIsPressed = true;
		break;
	case VK_SPACE:
		this->SPACE.bIsPressed = true;
		break;
	case KEY_Q:
		this->Q.bIsPressed = true;
		break;
	case KEY_E:
		this->E.bIsPressed = true;
		break;
	default:
		break;
	}
}

void Input::handle_mouse_movement(Engine* engine, LPARAM param)
{
	POINT p;

	 p.x = LOWORD(param);
	 p.y = HIWORD(param);

	ShowCursor(TRUE);
	engine->input.mouse_movement(engine, p.x, p.y);
}


#endif // WINDOWS

#if defined (GLFW) 

void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
		
	  	auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
			
			if(engine){
				float velocity = engine->main_camera.velocity;
				float value = yoffset;
				if(velocity <= 0.3){	
						value -= 0.5;
				}
				engine->main_camera.velocity += value;
			}

			
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
#if defined (VULKAN) || defined (DX11)
		auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
	
			if(key == GLFW_KEY_TAB){
				if(action == GLFW_PRESS){
					engine->input.TAB.bIsPressed = true;
					engine->input.TAB.Released = false;
				}
				if(action == GLFW_RELEASE){
					engine->input.TAB.bIsPressed = false;
					engine->input.TAB.Released = true;
				}
			}
			if(key == GLFW_KEY_S){
				if(action == GLFW_PRESS){
					engine->input.S.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.S.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_W){
				if(action == GLFW_PRESS){
					engine->input.W.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.W.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_D){
				if(action == GLFW_PRESS){
					engine->input.D.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.D.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_A){
				if(action == GLFW_PRESS){
					engine->input.A.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.A.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_Z){
				if(action == GLFW_PRESS){
					engine->input.Z.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.Z.bIsPressed = false;
				}
			}
			
			if(key == GLFW_KEY_E){
				if(action == GLFW_PRESS){
					engine->input.E.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.E.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_Q){
				if(action == GLFW_PRESS){
					engine->input.Q.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.Q.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_X){
				if(action == GLFW_PRESS){
					engine->input.X.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.X.bIsPressed = false;
				}
			}
			if(key == GLFW_KEY_V){
				if(action == GLFW_PRESS){
					engine->input.V.bIsPressed = true;
				}
				if(action == GLFW_RELEASE){
					engine->input.V.bIsPressed = false;
				}
			}
  #endif
           
}

void Input::mouse_callback(GLFWwindow* window, double xpos, double ypos){
	#if defined (VULKAN) || defined (DX11)
	  auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
	
		engine->input.mouse_movement(engine,xpos,ypos);
	 
	
	#endif

}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
#if defined (VULKAN) || defined (DX11)
		auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
	#endif

		if (button == GLFW_MOUSE_BUTTON_RIGHT ){
			if(action == GLFW_PRESS){
				engine->input.right_button_pressed = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
				engine->input.move_camera = false;
			}
			if(action == GLFW_RELEASE){
				engine->input.right_button_pressed = false;
			}

		}
		if (button == GLFW_MOUSE_BUTTON_LEFT ){
			if(action == GLFW_PRESS){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
				engine->input.move_camera = true; 
			}
			if(action == GLFW_RELEASE){
				//app->engine->input.right_button_pressed = false;
			}

		}

}
#endif//end if define vulkan

void Input::mouse_movement(Engine* engine, float xpos, float ypos){
	
	engine->input.mousex = xpos;
	engine->input.mousey = ypos;

	if(engine->input.move_camera){
		if(engine->input.first_mouse){
			engine->input.lastX = xpos;
			engine->input.lastY = ypos;
			engine->input.first_mouse = false;
		}

		float xoffset = xpos - engine->input.lastX ;

		float yoffset = engine->input.lastY - ypos; // reversed since y-coordinates range from bottom to top
	
		if(engine->edit_mode){
				xoffset *= 0.05;
				yoffset *= 0.05;
				engine->input.yaw   += xoffset;
				engine->input.pitch += yoffset;  
		}else{
			if(engine->input.lastX != xpos && engine->input.lastY != ypos ){	
				engine->input.yaw   = xoffset;
				engine->input.pitch = yoffset;  


			if(engine->input.pitch > 89.0f)
				engine->input.pitch =  89.0f;
			if(engine->input.pitch < -89.0f)
				engine->input.pitch = -89.0f;	


			}else{
				engine->input.yaw   = 0;
				engine->input.pitch = 0;  
			}
		}
		

		engine->input.lastX = xpos;
		engine->input.lastY = ypos;

		#ifdef ES2

		#endif

	
	}//end right click pressed
}	

#if defined(LINUX) && defined(X11)

void Input::check_input_event(Engine* engine, XEvent &xev){
	KeySym key;		
	char text[255];	
	KeySym key_release;		
	char key_release_char[255];	

	if ( xev.type == KeyPress ){
		//printf( "KeyPress: %i\n", xev.xkey.keycode );
		engine->input.key_code_verifier_pressed(xev.xkey.keycode);
	}

	if (xev.type == KeyRelease){
	// std::cout << "key realease from window manager \n";
	engine->input.key_code_verifier_released(xev.xkey.keycode);
	}		

	/* use the XLookupString routine to convert the invent
				KeyPress data into regular text.  Weird but necessary...
		*/
	if (xev.type==KeyPress&&
				XLookupString(&xev.xkey,text,255,&key,0)==1) {
		
				if (text[0]=='q') {
					
				}	
				engine->input.key_verifier_pressed(text[0]);

		}
	if (xev.type==KeyRelease &&
				XLookupString(&xev.xkey,key_release_char,255,&key_release,0)==1) {

				engine->input.key_verifier_released(key_release_char[0]);
		}

	if ( xev.type == MotionNotify ) {  // if mouse has moved
		engine->input.mouse_movement(engine,xev.xmotion.x,xev.xmotion.y);
		engine->input.mouse_in_gamex = xev.xmotion.x;
		engine->input.mouse_in_gamey = xev.xmotion.y;
	}
	else{
		engine->input.mouse_in_gamex = 600;
		engine->input.mouse_in_gamey = 800;
	}
	
	if(xev.type == ButtonPress){
		
		if(xev.xbutton.button == Button1){
		
				engine->input.left_button_pressed = true;
		}
		if(xev.xbutton.button == Button3){
			engine->input.right_button_pressed = true;
		}
	}
	
	if(xev.type == ButtonRelease){
		
		if(xev.xbutton.button == Button1){
				engine->input.left_button_pressed = false;
				engine->input.left_button_release = true;
		}
		if(xev.xbutton.button == Button3){
			engine->input.right_button_pressed = false;
			engine->input.right_button_release = true;
		}
	}

}

void Input::key_verifier_pressed(char character){	
	key_set(character,true);
}

void Input::key_verifier_released(char character){	
	key_set(character,false);
}

void Input::key_code_verifier_pressed(int key_code){
	
	
switch (key_code)
	{
	case 10:
		key_set('1',true);
		break;
	
	case 65:
		key_set('-',true);
		break;
	case 9:
		key_set('0',true);
		break;
	case 50:
		key_set('9', true);
		break;

	}
	
	


}
void Input::key_code_verifier_released(int key_code){
		

	switch (key_code)
	{
	case 10:
		key_set('1',false);
		break;
	
	case 65:
		key_set('-',false);
		break;

	case 9:
		key_set('0',false);
		break;
	case 50:
		key_set('9', false);
		break;
		
	}
	


}
void Input::key_set(const char key, bool isPressed){
	Key *actual_key;
	if(key == 'w'){
		actual_key = &this->W;
	}
	if(key == 's'){
		actual_key = &this->S;
	}
	if(key == 'd'){
		actual_key = &this->D;
	}
	if(key == 'a'){
		actual_key = &this->A;
	}
	if(key == 'q'){
		actual_key = &this->Q;
	}
	if(key == 'e'){
		actual_key = &this->E;
	}
	if(key == 'p'){
		actual_key = &this->TAB;	
	}
	if(key == 'z'){
		actual_key = &this->Z;
	}
	if(key == 'x'){
		actual_key = &this->X;
	}
	if(key == 'c'){
		actual_key = &this->C;
	}
	if(key == 'v'){
		actual_key = &this->V;
	}
	

	switch(key){
		case	'1':
			actual_key = &this->KEY_1;
			break;
		case '-':
			actual_key = &this->SPACE;
			break;
		case '0':
			actual_key = &this->ESC;
			break;
		case '9':
			actual_key = &this->SHIFT;
			break;
		case 'r':
			actual_key = &this->R;
			break;
	}

	if(actual_key){
		if(isPressed){
		actual_key->bIsPressed = true;
		}else{
			actual_key->bIsPressed = false;
			actual_key->Released = true;
		}
	}
}
#endif

void Input::update_input(Engine* engine){

	//std::cout << this->mouse_in_gamey << std::endl;
	//std::cout << this->mouse_in_gamex << std::endl;
	
	engine->input.mouse_in_gamex = 600;
	engine->input.mouse_in_gamey = 800;
	
	if(TAB.Released){
		if(!engine->edit_mode){
				engine->edit_mode = true;				
		}else{
			engine->edit_mode = false;
		}
		this->TAB.Released = false;
		
	}
	if(ESC.Released){
		engine->window_manager.close_window = true;
	}

	if(KEY_1.Released){
#ifdef DEVELOPMENT
		if(!engine->draw_gizmos){
				engine->draw_gizmos = true;				
		}else{
			engine->draw_gizmos = false;
		}
		this->KEY_1.Released = false;
#endif
	}

	if(engine->edit_mode){
		if(S.bIsPressed){
			engine->main_camera.MoveBackward();		
			
		}

		if(W.bIsPressed){
			engine->main_camera.MoveForward();
		}

		if(A.bIsPressed){		
			engine->main_camera.MoveLeft();
		}

		if(D.bIsPressed){		
			engine->main_camera.MoveRight();	
		}
#ifndef  ANDROID
		//float rotation_velocity = 0.2f;

		if(Z.bIsPressed){

			EMesh* mesh = engine->skeletal_meshes[0];
			Node* node = NodeManager::node_by_name(mesh,"bone2.003");
			node->rot_mat = rotate(node->rot_mat,radians(-15*engine->deltaTime),vec3(1,0,0));

		}

		if(X.bIsPressed){	

			EMesh* mesh = engine->skeletal_meshes[0];
			Node* node = NodeManager::node_by_name(mesh,"bone2");
			node->rot_mat = rotate(node->rot_mat,radians(15*engine->deltaTime),vec3(1,0,0));

		}

#endif
		if(C.bIsPressed){
			this->pitch += 0.4;
			engine->main_camera.mouse_control_update(yaw, pitch);
		}
		if(V.bIsPressed){
			//this->pitch -= 0.4;
			//engine->main_camera.mouse_control_update(yaw, pitch);

			engine->play_animations = true;
		}

		if(Q.bIsPressed){
			engine->main_camera.MoveDown();
		}
		if(E.bIsPressed){
			engine->main_camera.MoveUp();
		}

		if(move_camera){
			engine->main_camera.mouse_control_update(yaw, pitch);

		}		

		

	}
}

#ifdef ANDROID
void Input::input_event(AInputEvent* event){
    float x =  AMotionEvent_getX(event,0);
    float y = AMotionEvent_getY(event,0);
    this->mousex = x;
    this->mousey = y;
    LOGW("touch in x = %f y = %f",x,y);
}
#endif