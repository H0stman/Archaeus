#include <render.h>

//Extern variables
extern float background_colour[];

ID3D11Device* device_ptr = NULL;
ID3D11DeviceContext* device_context_ptr = NULL;
IDXGISwapChain* swap_chain_ptr = NULL;
ID3D11RenderTargetView* render_target_view_arr[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
ID3D11Texture2D* framebuffer = NULL;
ID3D11InputLayout* input_layout_ptr = NULL;
ID3D11Buffer* vertexbuffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT], * index = NULL;
static UINT StartSlot = 0, NumBuffers = 0;
static UINT strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT], offsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
static ID3D11VertexShader* vertex_shader_ptr = NULL;
ID3D11PixelShader* pixel_shader_ptr = NULL;
ID3DBlob* vs_blob_ptr = NULL, * ps_blob_ptr = NULL, * error_blob = NULL;
ID3D11Texture2D* DepthStencilBuffer = NULL;
ID3D11DepthStencilView* depth_view = NULL;
ID3D11DepthStencilState* depth_state = NULL;

void D3D11Initialize(HWND hndl)
{
	DXGI_SWAP_CHAIN_DESC swap_chain_descr =
	{
		.BufferDesc.RefreshRate.Numerator = 0,
		.BufferDesc.RefreshRate.Denominator = 1,
		.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
		.SampleDesc.Count = 1,
		.SampleDesc.Quality = 0,
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 2,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.OutputWindow = hndl,
		.Windowed = TRUE
	};

	D3D_FEATURE_LEVEL feature_level;
	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flags,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swap_chain_descr,
		&swap_chain_ptr,
		&device_ptr,
		&feature_level,
		&device_context_ptr);

	assert(S_OK == hr && swap_chain_ptr && device_ptr && device_context_ptr);

	hr = IDXGISwapChain_GetBuffer(swap_chain_ptr, 0u, &IID_ID3D11Texture2D, &framebuffer);
	assert(SUCCEEDED(hr));

	hr = ID3D11Device_CreateRenderTargetView(device_ptr, (ID3D11Resource*)framebuffer, NULL, render_target_view_arr);
	assert(SUCCEEDED(hr));

	RECT winRect;
	GetClientRect(hndl, &winRect);

	D3D11_TEXTURE2D_DESC desc =
	{
		desc.Width = winRect.right - winRect.left,
		desc.Height = winRect.bottom - winRect.top,
		desc.MipLevels = 1,
		desc.ArraySize = 1,
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
		desc.SampleDesc.Count = 1,
		desc.SampleDesc.Quality = 0,
		desc.Usage = D3D11_USAGE_DEFAULT,
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL,
		desc.CPUAccessFlags = 0,
		desc.MiscFlags = 0
	};

	hr = ID3D11Device_CreateTexture2D(device_ptr, &desc, 0, &DepthStencilBuffer);
	assert(SUCCEEDED(hr));

	hr = ID3D11Device_CreateDepthStencilView(device_ptr, (ID3D11Resource*)DepthStencilBuffer, NULL, &depth_view);
	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_DESC depthstencilDesc =
	{
		.DepthEnable = TRUE,
		.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		.DepthFunc = D3D11_COMPARISON_LESS_EQUAL
	};

	hr = ID3D11Device_CreateDepthStencilState(device_ptr, &depthstencilDesc, &depth_state);
	assert(SUCCEEDED(hr));

	flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#elif defined(NDEBUG)
	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif


	// COMPILE VERTEX SHADER
	hr = D3DCompileFromFile(
		L"../Shaders/vertex.hlsl",
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"vs_main",
		"vs_5_0",
		flags,
		0,
		&vs_blob_ptr,
		&error_blob);

	if (FAILED(hr))
	{
		if (error_blob)
		{
			OutputDebugStringA((char*)ID3D10Blob_GetBufferPointer(error_blob));
			ID3D10Blob_Release(error_blob);
		}
		if (vs_blob_ptr)
			ID3D10Blob_Release(vs_blob_ptr);
		assert(FALSE);
	}

	// COMPILE PIXEL SHADER
	hr = D3DCompileFromFile(
		L"../Shaders/pixel.hlsl",
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"ps_main",
		"ps_5_0",
		flags,
		0,
		&ps_blob_ptr,
		&error_blob);

	if (FAILED(hr))
	{
		if (error_blob)
		{
			OutputDebugStringA((char*)ID3D10Blob_GetBufferPointer(error_blob));
			ID3D10Blob_Release(error_blob);
		}
		if (ps_blob_ptr)
			ID3D10Blob_Release(ps_blob_ptr);
		assert(FALSE);
	}

	hr = ID3D11Device_CreateVertexShader(device_ptr, ID3D10Blob_GetBufferPointer(vs_blob_ptr), ID3D10Blob_GetBufferSize(vs_blob_ptr), NULL, &vertex_shader_ptr);
	assert(SUCCEEDED(hr));

	hr = ID3D11Device_CreatePixelShader(device_ptr, ID3D10Blob_GetBufferPointer(ps_blob_ptr), ID3D10Blob_GetBufferSize(ps_blob_ptr), NULL, &pixel_shader_ptr);
	assert(SUCCEEDED(hr));

	hr = IDXGISwapChain_GetBuffer(swap_chain_ptr, 0u, &IID_ID3D11Texture2D, (void**)&framebuffer);
	assert(SUCCEEDED(hr));

	hr = ID3D11Device_CreateRenderTargetView(device_ptr, (ID3D11Resource*)framebuffer, NULL, render_target_view_arr);
	assert(SUCCEEDED(hr));

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		/*{ "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		*/
	};

	hr = ID3D11Device_CreateInputLayout(device_ptr, inputElementDesc, ARRAYSIZE(inputElementDesc), ID3D10Blob_GetBufferPointer(vs_blob_ptr), ID3D10Blob_GetBufferSize(vs_blob_ptr), &input_layout_ptr);
	assert(SUCCEEDED(hr));

	float vertex_data_array[] = {
		1.0f,1.0f,-1.0f, 	0.0f, 0.0f, 0.0f,   // Vertex 0.
		-1.0f,1.0f,-1.0f, 0.0f, 0.0f, 1.0f,   // Vertex 1.
		-1.0f,1.0f,1.0f,  0.0f, 1.0f, 0.0f,   // And so on.
		1.0f,1.0f,1.0f,	0.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,	1.0f, 0.0f, 0.0f,
		-1.0f,-1.0f,-1.0f,1.0f, 0.0f, 1.0f,
		-1.0f,-1.0f,1.0f,	1.0f, 1.0f, 0.0f,
		1.0f,-1.0f,1.0f,	1.0f, 1.0f, 1.0f
	};

	strides[0] = 6 * sizeof(float);
	offsets[0] = 0;

	{
		D3D11_BUFFER_DESC vertex_buff_descr =
		{
			.ByteWidth = sizeof(vertex_data_array),
			.Usage = D3D11_USAGE_IMMUTABLE,
			.BindFlags = D3D11_BIND_VERTEX_BUFFER
		};

		D3D11_SUBRESOURCE_DATA sr_data =
		{
			.pSysMem = vertex_data_array
		};

		hr = ID3D11Device_CreateBuffer(device_ptr, &vertex_buff_descr, &sr_data, vertexbuffers);
		NumBuffers++;
		assert(SUCCEEDED(hr));
	}

	int index_data_array[] = {
		0u,1u,2u,                // Face 0 has three vertices.
		0u,2u,3u,                // And so on.
		0u,5u,1u,
		0u,4u,5u,
		1u,5u,6u,
		1u,6u,2u,
		2u,6u,7u,
		2u,7u,3u,
		3u,7u,4u,
		3u,4u,0u,
		4u,7u,6u,
		4u,6u,5u
	};


	D3D11_BUFFER_DESC index_buff_descr = {
		.ByteWidth = sizeof(index_data_array),
		.Usage = D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_INDEX_BUFFER
	};

	D3D11_SUBRESOURCE_DATA sr_data =
	{
		.pSysMem = index_data_array
	};

	hr = ID3D11Device_CreateBuffer(device_ptr, &index_buff_descr, &sr_data, &index);
	assert(SUCCEEDED(hr));

	D3D11_VIEWPORT viewport =
	{
		.TopLeftX = 0.0f,
		.TopLeftY = 0.0f,
		.Width = (float)(winRect.right - winRect.left),
		.Height = (float)(winRect.bottom - winRect.top),
		.MinDepth = 0.0f,
		.MaxDepth = 1.0f
	};
	ID3D11DeviceContext_RSSetViewports(device_context_ptr, 1u, &viewport);
	ID3D11DeviceContext_IASetIndexBuffer(device_context_ptr, index, DXGI_FORMAT_R32_UINT, 0u);
	ID3D11DeviceContext_IASetPrimitiveTopology(device_context_ptr, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ID3D11DeviceContext_IASetInputLayout(device_context_ptr, input_layout_ptr);
	ID3D11DeviceContext_IASetVertexBuffers(device_context_ptr, StartSlot, NumBuffers, vertexbuffers, strides, offsets);
	ID3D11DeviceContext_VSSetShader(device_context_ptr, vertex_shader_ptr, NULL, 0u);
	ID3D11DeviceContext_PSSetShader(device_context_ptr, pixel_shader_ptr, NULL, 0u);
}

void Render(void)
{
	ID3D11DeviceContext_OMSetRenderTargets(device_context_ptr, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, render_target_view_arr, depth_view);

	for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		if (render_target_view_arr[i] != NULL)
			ID3D11DeviceContext_ClearRenderTargetView(device_context_ptr, render_target_view_arr[i], background_colour);

	if (depth_view)
		ID3D11DeviceContext_ClearDepthStencilView(device_context_ptr, depth_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ID3D11DeviceContext_OMSetDepthStencilState(device_context_ptr, depth_state, 0);
	ID3D11DeviceContext_DrawIndexed(device_context_ptr, 36u, 0u, 0u);
	IDXGISwapChain_Present(swap_chain_ptr, 1u, 0u);
}

void Release(void)
{

	if (error_blob) ID3D10Blob_Release(error_blob);
	for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
	{
		if (render_target_view_arr[i])
			ID3D11RenderTargetView_Release(render_target_view_arr[i]);
		else
			break;
	}
	if (vertex_shader_ptr) ID3D11VertexShader_Release(vertex_shader_ptr);
	if (pixel_shader_ptr) ID3D11PixelShader_Release(pixel_shader_ptr);
	if (framebuffer) ID3D11Texture2D_Release(framebuffer);
	if (vs_blob_ptr) ID3D10Blob_Release(vs_blob_ptr);
	if (ps_blob_ptr) ID3D10Blob_Release(ps_blob_ptr);
	if (input_layout_ptr) ID3D11InputLayout_Release(input_layout_ptr);

	for (UINT i = 0; i < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; ++i)
		if (vertexbuffers[i])
			ID3D11Buffer_Release(vertexbuffers[i]);

	if (index) ID3D11Buffer_Release(index);
	if (device_context_ptr) ID3D11DeviceContext_Release(device_context_ptr);
	if (swap_chain_ptr) IDXGISwapChain_Release(swap_chain_ptr);
	if (DepthStencilBuffer) ID3D11Texture2D_Release(DepthStencilBuffer);
	if (depth_view) ID3D11DepthStencilView_Release(depth_view);
	if (depth_state) ID3D11DepthStencilState_Release(depth_state);
	if (device_ptr) ID3D11Device_Release(device_ptr);
	ID3D11DeviceContext_ClearState(device_context_ptr);
}