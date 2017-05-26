#include "stdafx.h"
#include "DirectXVault.h"

using namespace DirectX;


XMFLOAT4X4 Camera;

DirectXVault::DirectXVault()
{
	

}



DirectXVault::~DirectXVault()
{
	pipelineState.pixel_shader->Release();
	debugpipelineState.pixel_shader->Release();
	pipelineState.input_layout->Release();
	debugpipelineState.input_layout->Release();
	pipelineState.vertex_shader->Release();
	debugpipelineState.vertex_shader->Release();
	if (debugpipelineState.depthStencilView != nullptr)  debugpipelineState.depthStencilView->Release();
	if(pipelineState.depthStencilBuffer != nullptr)pipelineState.depthStencilBuffer->Release();
	if (debugpipelineState.depthStencilBuffer != nullptr)debugpipelineState.depthStencilBuffer->Release();
	if(pipelineState.depthStencilState!= nullptr)pipelineState.depthStencilState->Release();
	if (debugpipelineState.depthStencilState != nullptr) debugpipelineState.depthStencilState->Release();
	if(pipelineState.rasterState != nullptr)pipelineState.rasterState->Release();
	if (debugpipelineState.rasterState != nullptr)debugpipelineState.rasterState->Release();
	if (pipelineState.rasterState != nullptr)pipelineState.debugRasterState->Release();
	if (debugpipelineState.rasterState != nullptr)debugpipelineState.debugRasterState->Release();
	if(pipelineState.depthStencilView != nullptr) pipelineState.depthStencilView->Release();
	if (attribute.sampler != nullptr) attribute.sampler->Release();
	if (attribute.resourceView != nullptr) attribute.resourceView->Release();
	pipelineState.indexbuffer->Release();
	attribute.device->Release();
	if (attribute.device_context) attribute.device_context->Release();
	attribute.render_target->Release();
	attribute.swap_chain->Release();

	if (debugpipelineState.indexbuffer != nullptr)debugpipelineState.indexbuffer->Release();
	//debugattribute.device->Release();
	//if (debugattribute.device_context != nullptr) debugattribute.device_context->Release();
	//debugattribute.render_target->Release();
	//debugattribute.swap_chain->Release();

	matBuffer->Release();
	lineBufferx->Release();
	lineBuffery->Release();
	//lineBufferz->Release();
	GridBuffer->Release();
	IndexBuffer->Release();
	pointLightBuffer->Release();
	//lineBufferz->Release();
	whatyouplease->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	whatyouplease->Release();
}


void DirectXVault::Start(HWND window, /*std::vector<float>& _Position*/std::vector< DirectXVault::vertex>& _Position, std::vector<uint32_t>& indices) {

	wind = window;
	_indices = indices;
	Positions = _Position;
	//Bones = _Bones;
	UINT leDeviceFlags = 0;
#ifdef _DEBUG
	leDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL leFeatureArray[] = {
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
		//D3D_FEATURE_LEVEL_11_1,
	};
	UINT lefeatureLevelsAmount = ARRAYSIZE(leFeatureArray);


	// TODO: PART 1 STEP 3b
	DXGI_SWAP_CHAIN_DESC leSwapchainDesc;
	ZeroMemory(&leSwapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	leSwapchainDesc.BufferCount = 1;
	leSwapchainDesc.BufferDesc.Width = WIDTH;
	leSwapchainDesc.BufferDesc.Height = HEIGHT;
	leSwapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	leSwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	leSwapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
	leSwapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
	leSwapchainDesc.OutputWindow = wind;
	leSwapchainDesc.SampleDesc.Count = 1;
	leSwapchainDesc.Windowed = true;
	leSwapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT Result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		leDeviceFlags, leFeatureArray, lefeatureLevelsAmount, D3D11_SDK_VERSION, &leSwapchainDesc,
		&attribute.swap_chain, &attribute.device, &attribute.feature_levels, &attribute.device_context);

	ID3D11Texture2D * leRenderTargetTemp = 0;
	attribute.swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&leRenderTargetTemp);
	attribute.device->CreateRenderTargetView(leRenderTargetTemp, nullptr, &attribute.render_target);
	leRenderTargetTemp->Release();

	attribute.device_context->OMSetRenderTargets(1,&attribute.render_target,nullptr);

	ZeroMemory(&attribute.viewport, sizeof(D3D11_VIEWPORT));
	attribute.viewport.Width = WIDTH;
	attribute.viewport.Height = HEIGHT;
	attribute.viewport.MaxDepth = 1.0f;
	attribute.viewport.MinDepth = 0.0f;
	attribute.viewport.TopLeftX = 0;
	attribute.viewport.TopLeftY = 0;


	attribute.device_context->RSSetViewports(1, &attribute.viewport);
	
	D3D11_RASTERIZER_DESC desc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	attribute.device->CreateRasterizerState(&desc, &pipelineState.rasterState);
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;
	attribute.device->CreateRasterizerState(&desc, &pipelineState.debugRasterState);

	debugattribute = attribute;
	attribute.device->QueryInterface(IID_PPV_ARGS(&whatyouplease));
	SetUpShadersForACoolTriangle();
	BufferUpTheTriangle();
	BufferUpTheLines();
	BufferUpTheGrid();
	GimmeACamera();
	XMStoreFloat4x4(&matrix.translation, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&matrix.view,DirectX::XMMatrixIdentity());
	//XMStoreFloat4x4(&matrix.projection, DirectX::XMMatrixIdentity());

}

void DirectXVault::Render() {

	UpdateCamera();
	//XMStoreFloat4x4(&matrix.translation, XMMatrixTranspose(DirectX::XMMatrixIdentity()));
	if (GetAsyncKeyState('Q') & 0x1)
	{
		RandomizeStuff();
	}
	leMatrix copy = matrix;


	XMStoreFloat4x4(&copy.view , XMMatrixInverse(nullptr, XMLoadFloat4x4(&copy.view)));
	
	XMStoreFloat4x4(&copy.view, XMMatrixTranspose(XMMatrixInverse(nullptr,  XMLoadFloat4x4(&cam))));
	transPose4X4(copy.view);
	transPose4X4(copy.translation);
	transPose4X4(copy.projection);

	attribute.device_context->UpdateSubresource(matBuffer, 0, NULL, &copy, 0, 0);
	attribute.device_context->VSSetConstantBuffers(0, 1, &matBuffer);

	debugattribute.device_context->UpdateSubresource(matBuffer, 0, NULL, &copy, 0, 0);
	debugattribute.device_context->VSSetConstantBuffers(0, 1, &matBuffer);
//	attribute.device_context->ClearDepthStencilView(pipelineState.depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	attribute.device_context->ClearRenderTargetView(attribute.render_target, DirectX::Colors::SeaGreen);
	attribute.device_context->IASetInputLayout(pipelineState.input_layout);

	//debugattribute.device_context->ClearRenderTargetView(debugattribute.render_target, DirectX::Colors::SeaGreen);
	debugattribute.device_context->IASetInputLayout(debugpipelineState.input_layout);

	DrawTheCoolestTriangle();
	DrawTheCoolestGrid();
	DrawTheCoolestLines();
	attribute.swap_chain->Present(0, 0);
}

void DirectXVault::SetUpShadersForACoolTriangle() {
	
	attribute.device->CreatePixelShader(trivial_PS, sizeof(trivial_PS), NULL, &pipelineState.pixel_shader);
	attribute.device->CreateVertexShader(trivial_VS, sizeof(trivial_VS), NULL, &pipelineState.vertex_shader);

	debugattribute.device->CreatePixelShader(trivial_PS, sizeof(trivial_PS), NULL, &debugpipelineState.pixel_shader);
	debugattribute.device->CreateVertexShader(trivial_VS, sizeof(trivial_VS), NULL, &debugpipelineState.vertex_shader);
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	attribute.device->CreateInputLayout(ied,2,trivial_VS,sizeof(trivial_VS),&pipelineState.input_layout);
	debugattribute.device->CreateInputLayout(ied, 2, trivial_VS, sizeof(trivial_VS), &debugpipelineState.input_layout);


}

void DirectXVault::BufferUpTheTriangle() {
	vertex OurVertices[] =
	{
		{DirectX::XMFLOAT4(0.0f, 0.5f, 0.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{ DirectX::XMFLOAT4(0.45f, -0.5, 0.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.45f, -0.5f, 0.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
	};


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(vertex) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	attribute.device->CreateBuffer(&bd, NULL, &pipelineState.indexbuffer);       // create the buffer


												   // copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	attribute.device_context->Map(pipelineState.indexbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	attribute.device_context->Unmap(pipelineState.indexbuffer, NULL);                                      // unmap the buffer
}

void DirectXVault::BufferUpTheGrid() {
	vertex OurVertices[] =
	{
		 { DirectX::XMFLOAT4(-10.0f, 0.0f,-10.0f,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(-10.0f, 0.0f, 10.0f,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-8.0f, 0.0f,-10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(-8.0f, 0.0f, 10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },
										   
		 { DirectX::XMFLOAT4(-6.0f, 0.0f,-10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f) },
		 { DirectX::XMFLOAT4(-6.0f, 0.0f, 10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f) },
										   
		 { DirectX::XMFLOAT4(-4.0f, 0.0f,-10.0f,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(-4.0f, 0.0f, 10.0f,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },
										   
		 { DirectX::XMFLOAT4(-2.0f, 0.0f,-10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(-2.0f, 0.0f, 10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },
										   
		 { DirectX::XMFLOAT4(0.0f,  0.0f, -10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f) },
		 { DirectX::XMFLOAT4(0.0f, 0.0f, 10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f) },
										   
		 { DirectX::XMFLOAT4(2.0f,  0.0f, -10.0f,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(2.0f, 0.0f, 10.0f,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },
										   
		 { DirectX::XMFLOAT4(4.0f,  0.0f, -10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(4.0f, 0.0f, 10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },
										   
		 { DirectX::XMFLOAT4(6.0f,  0.0f, -10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f) },
		 { DirectX::XMFLOAT4(6.0f, 0.0f, 10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f) },
										   
		 { DirectX::XMFLOAT4(8.0f,  0.0f, -10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(8.0f, 0.0f, 10.0f,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },
										   
		 { DirectX::XMFLOAT4(10.0f,  0.0f, -10.0f,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(10.0f, 0.0f, 10.0f,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },

		 ///////////////////////////////////////////////////////////////////////////////////////////

		 { DirectX::XMFLOAT4(-10.0f, 0.0f, -10.0f,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(10.0f, 0.0f, -10.0f,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-10.0f, 0.0f, -8.0f,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(10.0f, 0.0f, -8.0f,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-10.0f, 0.0f, -6.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f) },
		 { DirectX::XMFLOAT4(10.0f, 0.0f, -6.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-10.0f, 0.0f, -4.0f,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(10.0f, 0.0f, -4.0f,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-10.0f, 0.0f, -2.0f,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(10.0f, 0.0f, -2.0f,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-10.0f, 0.0f, 0.0f ,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f) },
		 { DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f ,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-10.0f, 0.0f, 2.0f ,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(10.0f, 0.0f, 2.0f ,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-10.0f, 0.0f, 4.0f ,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(10.0f, 0.0f, 4.0f ,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-10.0f, 0.0f, 6.0f ,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f) },
		 { DirectX::XMFLOAT4(10.0f, 0.0f, 6.0f ,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-10.0f, 0.0f, 8.0f ,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(10.0f, 0.0f, 8.0f ,1.0f),DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f) },

		 { DirectX::XMFLOAT4(-10.0f, 0.0f, 10.0f ,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },
		 { DirectX::XMFLOAT4(10.0f, 0.0f, 10.0f ,1.0f),DirectX::XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f) },

	};


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(vertex) * 44;    // Positions.size() /3 size is the VERTEX struct * 2 (6)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	D3D11_SUBRESOURCE_DATA SUBRESx;
	SUBRESx.pSysMem = OurVertices;
	SUBRESx.SysMemPitch = 0;
	SUBRESx.SysMemSlicePitch = 0;

	attribute.device->CreateBuffer(&bd, &SUBRESx, &GridBuffer);       // create the buffer


	CD3D11_BUFFER_DESC bdm(sizeof(leMatrix), D3D11_BIND_CONSTANT_BUFFER);
	attribute.device->CreateBuffer(&bdm, NULL, &matBuffer);

	CD3D11_BUFFER_DESC pointLightConstantBufferDesc(sizeof(LightInfo), D3D11_BIND_CONSTANT_BUFFER);
	attribute.device->CreateBuffer(&pointLightConstantBufferDesc, nullptr, &pointLightBuffer);
	lightData.lightColor = { 1.0f,1.0f,1.0f,1.0f };
	lightData.lightDirection = { 0.0f,1.0f,1.0f };
	lightData.lightPosition = { 0.0f,0.0f,-10.0f };
	lightData.radius = 30.0f;

}

void DirectXVault::BufferUpTheLines() {
	

	
	std::vector<vertex>  OurVerticesx;// = new std::vector<vertex>;
	std::vector<vertex>  OurVertices2;// = new std::vector<vertex>;

	
	for (int i = 0; i < Positions.size(); i++)
	{
		vertex v;
		v.Position = Positions[i].Position;
		v.Color = Positions[i].Color;
		v.Normals = Positions[i].Normals;
		OurVerticesx.push_back(v);

	}

	/*for (int i = 0; i < OurVerticesx.size() / 3; i++)
	{
		OurVertices2.push_back(OurVerticesx[i*3 +0]);
		OurVertices2.push_back(OurVerticesx[i*3 +1]);
		OurVertices2.push_back(OurVerticesx[i*3 +1]);
		OurVertices2.push_back(OurVerticesx[i*3 +2]);
		OurVertices2.push_back(OurVerticesx[i*3 +2]);
		OurVertices2.push_back(OurVerticesx[i*3 +0]);
	}
*/


	sizetodraw = OurVerticesx.size();
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	D3D11_SAMPLER_DESC samdes;
	ZeroMemory(&samdes, sizeof(samdes));
	samdes.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samdes.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samdes.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samdes.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	attribute.device->CreateSamplerState(&samdes, &attribute.sampler);

	DirectX::CreateWICTextureFromFile(attribute.device, attribute.device_context, L"Teddy_D.png", NULL, &attribute.resourceView);

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(vertex) * OurVerticesx.size();    // Positions.size() /3 size is the VERTEX struct * 2 (6)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	D3D11_SUBRESOURCE_DATA SUBRESx;
	SUBRESx.pSysMem = OurVerticesx.data();
	SUBRESx.SysMemPitch = 0;
	SUBRESx.SysMemSlicePitch = 0;

	attribute.device->CreateBuffer(&bd, &SUBRESx, &lineBufferx);       // create the buffer


	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = _indices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(uint32_t) *  _indices.size(), D3D11_BIND_INDEX_BUFFER);
	attribute.device->CreateBuffer(&indexBufferDesc, &indexBufferData, &IndexBuffer);
	//nRenderable->indexCount = indexCount;

}

void DirectXVault::bufferdemBones(std::vector<float>& _Bones) {
	//bones
	Bones.clear();
	Bones = _Bones;
	std::vector<vertex> BonesVerts;

	for (int i = 0; i < Bones.size(); i += 4)
	{
		vertex v;
		v.Position.x = Bones[i];
		v.Position.y = Bones[i + 1];
		v.Position.z = Bones[i + 2];
		v.Position.w = 1.0f; //Positions[i + 3];

		v.Color = { 1.0f,1.0f,1.0f,1.0f };
		BonesVerts.push_back(v);

	}

	sizetodrawBones = BonesVerts.size();

	if (lineBuffery == nullptr)
	{

	D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));

	bd2.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd2.ByteWidth = sizeof(vertex) * BonesVerts.size();    // Positions.size() /3 size is the VERTEX struct * 2 (6)
	bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	debugattribute.device->CreateBuffer(&bd2, NULL, &lineBuffery);       // create the buffer

	}
																		 //D3D11_SUBRESOURCE_DATA SUBRESy;
																		 //SUBRESy.pSysMem = BonesVerts.data();
																		 //SUBRESy.SysMemPitch = 0;
																		 //SUBRESy.SysMemSlicePitch = 0;

	D3D11_MAPPED_SUBRESOURCE ms;
	debugattribute.device_context->Map(lineBuffery, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, BonesVerts.data(), BonesVerts.size() * sizeof(vertex));                 // copy the data
	debugattribute.device_context->Unmap(lineBuffery, NULL);



	//CD3D11_BUFFER_DESC bdm(sizeof(leMatrix), D3D11_BIND_CONSTANT_BUFFER);
	//debugattribute.device->CreateBuffer(&bdm, NULL, &matBuffer);
}
void DirectXVault::DrawTheCoolestTriangle() {



	attribute.device_context->PSSetShader(pipelineState.pixel_shader, NULL, 0);
	attribute.device_context->VSSetShader(pipelineState.vertex_shader, NULL, 0);
	// select which vertex buffer to display
	UINT stride = sizeof(vertex);
	UINT offset = 0;
	attribute.device_context->IASetVertexBuffers(0, 1, &pipelineState.indexbuffer, &stride, &offset);

	// select which primtive type we are using
	attribute.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer
	attribute.device_context->Draw(3, 0);

}

void DirectXVault::DrawTheCoolestLines() {
	
	if (toggle) {
		attribute.device_context->RSSetState(pipelineState.rasterState);
	}
	else
		attribute.device_context->RSSetState(pipelineState.debugRasterState);

	////body////
	attribute.device_context->PSSetShader(pipelineState.pixel_shader, NULL, 0);
	attribute.device_context->VSSetShader(pipelineState.vertex_shader, NULL, 0);

	//// select which vertex buffer to display
	UINT stride = sizeof(vertex);
	UINT offset = 0;
	//// select which primtive type we are using
	attribute.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//// draw the vertex buffer to the back buffer
	attribute.device_context->IASetVertexBuffers(0, 1, &lineBufferx, &stride, &offset);
	attribute.device_context->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	attribute.device_context->PSSetShaderResources(0, 1, &attribute.resourceView);
	attribute.device_context->PSSetSamplers(0, 1, &attribute.sampler);

	attribute.device_context->DrawIndexed(_indices.size(), 0,0);

	if (!toggle)
	{
		///// bones /////
		debugattribute.device_context->PSSetShader(debugpipelineState.pixel_shader, NULL, 0);
		debugattribute.device_context->VSSetShader(debugpipelineState.vertex_shader, NULL, 0);
		//// select which vertex buffer to display
		stride = sizeof(vertex);
		offset = 0;
		//// select which primtive type we are using
		attribute.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		//// draw the vertex buffer to the back buffer
		attribute.device_context->IASetVertexBuffers(0, 1, &lineBuffery, &stride, &offset);
		
		attribute.device_context->Draw(sizetodrawBones, 0);
	}
	



}

void DirectXVault::DrawTheCoolestGrid() {
	attribute.device_context->PSSetShader(pipelineState.pixel_shader, NULL, 0);
	attribute.device_context->VSSetShader(pipelineState.vertex_shader, NULL, 0);
	//// select which vertex buffer to display
	UINT stride = sizeof(vertex);
	UINT offset = 0;
	//// select which primtive type we are using
	attribute.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//// draw the vertex buffer to the back buffer
	attribute.device_context->IASetVertexBuffers(0, 1, &GridBuffer, &stride, &offset);
	attribute.device_context->Draw(44, 0);
}



void DirectXVault::GimmeACamera() {

	DirectX::XMVECTORF32 eye = { 0.5f, 0.7f, -1.5f, 0.0f };
	DirectX::XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&cam, XMMatrixInverse(nullptr,XMMatrixLookAtLH(eye, at, up)));
	XMStoreFloat4x4(&matrix.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
	float aspectRatio = WIDTH / HEIGHT;
	float fovAngleY = 70.0f * 3.14f / 180.0f;

	DirectX::XMMATRIX projj = DirectX::XMMatrixPerspectiveFovLH(fovAngleY,aspectRatio,0.01f,1000.0f);
	XMStoreFloat4x4(&matrix.projection, projj);
	
	//XMStoreFloat4x4(&matrix.translation, XMMatrixInverse(nullptr, XMMatrixLookAtLH(eye, at, up)));
	
	
}

void DirectXVault::RandomizeStuff() {

	DirectX::XMMATRIX model = DirectX::XMLoadFloat4x4(&matrix.translation);

	float rX = (float)rand() / RAND_MAX * 2.0f - 1.0f;
	float rY = (float)rand() / RAND_MAX * 2.0f - 1.0f;
	float rZ = (float)rand() / RAND_MAX * 2.0f - 1.0f;

	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslation(rX, rY, rZ);

	DirectX::XMMATRIX rotatex = DirectX::XMMatrixRotationX((rand() % 360)*DirectX::XM_PI  /180.0f);
	DirectX::XMMATRIX rotatey = DirectX::XMMatrixRotationY((rand() % 360)*DirectX::XM_PI / 180.0f);
	DirectX::XMMATRIX rotatez = DirectX::XMMatrixRotationZ((rand() % 360)*DirectX::XM_PI / 180.0f);

	model = rotatex;
	model *= rotatey;
	model *= rotatez;
	model *= trans;
	DirectX::XMStoreFloat4x4(&matrix.translation, DirectX::XMMatrixTranspose(model));
}


void DirectXVault::SetMousePos() {
	/*
	OldMousePos = NewMousePos;
	GetCursorPos(&NewMousePos);
	ScreenToClient(wind, &NewMousePos);
	*/
	POINT p;
	p.x = WIDTH / 2;
	p.y = HEIGHT / 2;

	ClientToScreen(wind, &p);
	SetCursorPos(p.x, p.y);
}
void DirectXVault::UpdateCamera() {

	GetCursorPos(&currPoint);


	if ( GetAsyncKeyState('W'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, -0.5f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);

		/*matrix.view._34 -= 0.05f;*/

	}
	if (GetAsyncKeyState('S'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, 0.5f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
		/*matrix.view._34 += 0.05f;*/
	}
	if (GetAsyncKeyState('A'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.5f, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);

		/*matrix.view._14 += 0.05f;*/
	}
	if (GetAsyncKeyState('D'))
	{
		XMMATRIX translation = XMMatrixTranslation(-0.5f, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);

		/*matrix.view._34 -= 0.05f;*/
	}
	if (GetAsyncKeyState('X'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.5f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
		/*matrix.view._24 += 0.05f;*/
	}



	if (GetAsyncKeyState('B'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, -0.5f, 0.0f * 10);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);

		/*matrix.view._24 -= 0.05f;*/
	}

	
		
		//OldMousePos = NewMousePos;



	/*if (GetAsyncKeyState( 'W'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, 0.0005f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);

	}
	if (GetAsyncKeyState('S'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, -0.0005f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
	}
	if (GetAsyncKeyState('A'))
	{
		XMMATRIX translation = XMMatrixTranslation(-0.0005f, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
	}
	if (GetAsyncKeyState('D'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0005f, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
	}
	if (GetAsyncKeyState('X'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, -0.0005f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&matrix.translation, result);
	}

	if (GetAsyncKeyState(VK_UP))
	{
		XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.0005f, 0.0f);
		XMMATRIX Temp = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX Result = XMMatrixMultiply(Translation, Temp);
		XMStoreFloat4x4(&matrix.translation, Result);
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		XMMATRIX Translation = XMMatrixTranslation(0.0f, -0.0005f, 0.0f);
		XMMATRIX Temp = XMLoadFloat4x4(&matrix.translation);
		XMMATRIX Result = XMMatrixMultiply(Translation, Temp);
		XMStoreFloat4x4(&matrix.translation, Result);
	}*/
	/*POINT pos;
	GetCursorPos(&pos);*/
	if (GetKeyState(VK_RBUTTON)<0)
	{

		//ScreenToClient(wind, &pos);
		//MousePos = pos;
		float dx = (float)(currPoint.x - prevPoint.x) * 0.01f;
		float dy = (float)(currPoint.y - prevPoint.y) * 0.01f;

		printf("%f, %f\n", dx, dy);

		XMFLOAT4 Pos = XMFLOAT4(cam._41, cam._42, cam._43, cam._44);

		cam._41 = 0.0f;
		cam._42 = 0.0f;
		cam._43 = 0.0f;

		XMMATRIX RotationX = XMMatrixRotationX(dy*0.05f);
		XMMATRIX RotationY = XMMatrixRotationY(dx*0.05f);

		XMMATRIX Temp = XMLoadFloat4x4(&cam);
		Temp = XMMatrixMultiply(RotationX, Temp);
		Temp = XMMatrixMultiply(Temp, RotationY);

		XMStoreFloat4x4(&cam, Temp);

		cam._41 = Pos.x;
		cam._42 = Pos.y;
		cam._43 = Pos.z;

		//SetMousePos();
	}

	prevPoint = currPoint;

}

void DirectXVault::KeyPressed(bool ispressed) {
	RMouseClick = ispressed;
}
void DirectXVault::transPose4X4(XMFLOAT4X4 & conv) {


	XMFLOAT4X4 temp = conv;

	conv._11 = temp._11; conv._12 = temp._21; conv._13 = temp._31; conv._14 = temp._41;
	conv._21 = temp._12; conv._22 = temp._22; conv._23 = temp._32; conv._24 = temp._42;
	conv._31 = temp._13; conv._32 = temp._23; conv._33 = temp._33; conv._34 = temp._43;
	conv._41 = temp._14; conv._42 = temp._24; conv._43 = temp._34; conv._44 = temp._44;


}

POINT DirectXVault::getCurrMousePos() {
	return MousePos;
}

void DirectXVault::setCurrMouse(POINT p) {
	 MousePos = p;
}