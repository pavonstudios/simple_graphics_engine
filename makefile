CC=g++ -g -std=c++17
Library=-lglfw -lpthread 

MAIN_OBJS = main.cpp camera.cpp engine.cpp gltf.cpp
DEFINES= -DGLTF -DTINYGLTF_NO_EXTERNAL_IMAGE

.ONESHELL:
main:
	mkdir -p bin && cd src
	$(CC) $(MAIN_OBJS) renderer.cpp -o ../renderer $(Library) -I./ -lvulkan  -DVULKAN $(DEFINES)

renderer.o:
	$(CC) -c renderer.cpp -I./

.ONESHELL:
gl:
	mkdir -p bin && cd src
	$(CC) $(MAIN_OBJS) opengl_renderer.cpp -o ../renderer $(Library) -lGLEW -lGL -I./ -D_OpenGL_Renderer_
