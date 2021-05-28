#include <Renderer.h>

ID3D11Device* device_ptr = NULL;
ID3D11DeviceContext* device_context_ptr = NULL;
IDXGISwapChain* swap_chain_ptr = NULL;
ID3D11RenderTargetView* render_target_view_ptr = NULL;
ID3D11Texture2D* framebuffer = NULL;
ID3D11InputLayout* input_layout_ptr = NULL;
ID3D11Buffer* vertexbuffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT], * index = NULL;
UINT StartSlot = 0, NumBuffers = 0;
UINT strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT], offsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
ID3D11VertexShader* vertex_shader_ptr = NULL;
ID3D11PixelShader* pixel_shader_ptr = NULL;
ID3DBlob* vs_blob_ptr = NULL, * ps_blob_ptr = NULL, * error_blob = NULL;


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

	hr = ID3D11Device_CreateRenderTargetView(device_ptr, (ID3D11Resource*)framebuffer, NULL, &render_target_view_ptr);
	assert(SUCCEEDED(hr));
	ID3D11Texture2D_Release(framebuffer);

	flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG; // add more debug output
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

	hr = ID3D11Device_CreateRenderTargetView(device_ptr, (ID3D11Resource*)framebuffer, NULL, &render_target_view_ptr);
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
		1.0f,1.0f,-1.0f, 	0.0f, 0.0f, 1.0f,   // Vertex 0.
		-1.0f,1.0f,-1.0f, 0.0f, 1.0f, 0.0f,   // Vertex 1.
		-1.0f,1.0f,1.0f,  1.0f, 0.0f, 0.0f,   // And so on.
		1.0f,1.0f,1.0f,	0.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,	1.0f, 0.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,1.0f,	0.0f, 0.0f, 0.0f,
		1.0f,-1.0f,1.0f,	1.0f, 1.0f, 0.0f
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

		hr = ID3D11Device_CreateBuffer(device_ptr, &vertex_buff_descr, &sr_data, &vertexbuffers[0]);
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


	RECT winRect;
	GetClientRect(hndl, &winRect);
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

void Release(void)
{
	ID3D11RenderTargetView_Release(render_target_view_ptr);
	ID3D11VertexShader_Release(vertex_shader_ptr);
	ID3D11PixelShader_Release(pixel_shader_ptr);
	ID3D11Texture2D_Release(framebuffer);
	ID3D10Blob_Release(vs_blob_ptr);
	ID3D10Blob_Release(ps_blob_ptr);
	ID3D11InputLayout_Release(input_layout_ptr);
	ID3D11Buffer_Release(vertexbuffers[0]);
	ID3D11DeviceContext_Release(device_context_ptr);
	ID3D11Device_Release(device_ptr);
	IDXGISwapChain_Release(swap_chain_ptr);
}