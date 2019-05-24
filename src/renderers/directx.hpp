#ifndef DIRECTX
#define DIRECTX
#include <d3d11.h>
#include <d3dcompiler.h>


#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

class Engine;

class Renderer{
public:
	Engine* engine;
	// global declarations
	IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
	ID3D11Device* dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
	ID3D11RenderTargetView* backbuffer;

	ID3D11Buffer* pVBuffer;

	ID3D11InputLayout* pLayout;

	void init();
	void draw_frame();
	void init_pipeline();
	void create_buffer(ID3D11Buffer **buffer);
	HRESULT create_shader(LPCWSTR path, ID3DBlob** shader_blob, LPCSTR type, LPCSTR profile);
};
#endif