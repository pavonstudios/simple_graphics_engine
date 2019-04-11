CLANG=clang++ -g -std=c++17 -stdlib=libc++
CC=g++ -g -std=c++17
Library=-lglfw -lpthread -lm

MAIN_OBJS = camera.cpp engine.cpp gltf.cpp
DEFINES= -DGLTF

OBJs= main.o renderer.o engine.o camera.o asset_manager.o 3D_objects.o input.o

.ONESHELL:
renderer: renderer.o engine.o camera.o 3D_objects.o input.o
	mkdir -p bin && cd src
	$(CC) -o ../renderer $(OBJs) model_loader.o $(Library) -I./ -lvulkan  -DVULKAN $(DEFINES)

.ONESHELL:
input.o: 
	cd src
	$(CC) -c input.cpp -DGLTF -DVULKAN 

.ONESHELL:
3D_objects.o:
	cd src
	$(CC) -c 3D_objects.cpp -DGLTF -DVULKAN 

.ONESHELL:
full: $(OBJs)
	mkdir -p bin && cd src
	$(CC) -o ../renderer main.o renderer.o engine.o camera.o asset_manager.o 3D_objects.o model_loader.o $(Library) -I./ -lvulkan  -DVULKAN $(DEFINES)

.ONESHELL:
main.o:
	cd src
	$(CC) -c main.cpp -DGLTF -DVULKAN 

.ONESHELL:
renderer.o:
	cd src
	$(CC) -c renderer.cpp -I./ -DGLTF -DVULKAN

.ONESHELL:
asset_manager.o:
	cd src
	$(CC) -c asset_manager.cpp -I./ -DGLTF -DVULKAN

.ONESHELL:
engine.o: 
	cd src
	$(CC) -c engine.cpp -I./ -DGLTF -DVULKAN
	
.ONESHELL:
camera.o:
	cd src
	$(CC) -c camera.cpp gltf.cpp -I./ -DGLTF -DVULKAN

.ONESHELL:
gl:
	mkdir -p bin && cd src
	$(CC) $(MAIN_OBJS) opengl_renderer.cpp -o ../renderer $(Library) -lGLEW -lGL -I./ -D_OpenGL_Renderer_

.ONESHELL:
clang:
	mkdir -p bin && cd src
	$(CLANG) $(MAIN_OBJS) renderer.cpp -o ../renderer $(Library) -I./ -lvulkan  -DVULKAN $(DEFINES)
