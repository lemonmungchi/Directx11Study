#include "pch.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_width = GWinSizeX;
	_height = GwinSizeY;


	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	//�ﰢ�� �׸��� ��Ʈ
	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();

	CreateSRV();
	CreateConstantBuffer();
}

void Game::Update()
{
	_transformData.offset.x = 0.3f;
	_transformData.offset.y = 0.3f;

	D3D11_MAPPED_SUBRESOURCE subResouce;
	ZeroMemory(&subResouce, sizeof(subResouce));

	//���� ���� ���� �־��� �غ� �Ѵ�. ���� ���� �ְ�(�������ְ�) UNMAP
	_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResouce);
	::memcpy(subResouce.pData,&_transformData,sizeof(_transformData));			//�ٷ� cpu-> gpu �Ѿ�� �ȴ�.
	_deviceContext->Unmap(_constantBuffer.Get(), 0);
}

void Game::Render()
{
	RenderBegin();			//�ʱ�ȭ

	// IA - VS - RS - PS -OM
	//TODO : �׸���
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;
		//IA - ���úκ�
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(),&stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT,0);
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		//�ﰢ������ ������ֱ�

		//VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);		//�̰ɷ� ���ϰ� 
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
		
		//RS

		//PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());		//0�����Կ� 1��
		//OM

		//_deviceContext->Draw(_vertices.size(), 0);
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

	RenderEnd();			//����
}


void Game::RenderBegin()		//������ ���������ο� �츮�� ����� �����ֱ�
{
	//����ٰ� �׸����̴�.
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);				//OM: OUTPUT Mege
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);		//�������� �ʱ�ȭ���ֱ�
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);			//���� ���� <= �ĸ� 
	CHECK(hr);
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));		//�� 0���� �ʱ�ȭ���� �ʿ��� �͸� ���� �ʱ�ȭ
	{
		desc.BufferDesc.Width = _width;		//���۵� ���� ũ��� 800 x 600
		desc.BufferDesc.Height = _height;
		desc.BufferDesc.RefreshRate.Numerator = 60;			//ȭ�� �ֻ���
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;			//��Ƽ ���ø� �������, ��׷��� ��� ó������ 
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			//���۸� ��� ������ - �׷��ִ� ��Ȱ
		desc.BufferCount = 1;		//���۸
		desc.OutputWindow = _hwnd;		// ��� �������ڵ�
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}
	//HRESULT ������ bool��
	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,			//��� ������ -> gpu or software�� gpu
		nullptr,
		0,
		nullptr,				//�����ϴ� dx ���� ���� �⺻���� ���� ������ �� �ִ� �������� ����� 
		0,						//���� �迭 ũ��
		D3D11_SDK_VERSION,				//��ũ��
		&desc,
		_swapChain.GetAddressOf(),			//**�̸� �ּҰ� �������� GetAddressOf()
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	CHECK(hr);
}

void Game::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());		//����� �ؽ�ó(PNG)�� ��������
	CHECK(hr);

	//�ý�ó��°� �׳� ���� �ƹ��͵� ���µ� �ű⿡ ���� �׸��� �׸��� ���� ���Ŷ�� �±׸� �޾��ִ� �� 
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());  //����Ÿ�Ϻ��°����� �ؽ�ó�� ����� ������ش�.
	CHECK(hr);
	
}

void Game::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}

void Game::CreateGeometry()
{
	//��������
	{
		_vertices.resize(4);

		//13 -> 012
		//02 -> 213
		_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f);
		_vertices[0].uv = Vec2(0.f, 1.f);
		//_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);
		_vertices[1].position = Vec3(-0.5f, 0.5f, 0.f);
		_vertices[1].uv = Vec2(0.f, 0.f);
		//_vertices[1].color = Color(1.f, 0.f, 0.f, 1.f);
		_vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
		_vertices[2].uv = Vec2(1.f, 1.f);
		//_vertices[2].color = Color(1.f, 0.f, 0.f, 1.f);
		_vertices[3].position = Vec3(0.5f, 0.5f, 0.f);
		_vertices[3].uv = Vec2(1.f, 0.f);
		//_vertices[3].color = Color(1.f, 0.f, 0.f, 1.f);
	}

	//��������
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;			//gpu�� read only
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();			//ù��° �����ּ� cpu���� ����ȴ�.

		HRESULT hr = _device->CreateBuffer(&desc,&data, _vertexBuffer.GetAddressOf());
		CHECK(hr);
	}

	//Index
	{
		_indices = { 0,1,2,2,1,3 };
	}

	//IndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;			//gpu�� read only
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(uint32) * _indices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data();			//ù��° �����ּ� cpu���� ����ȴ�.

		HRESULT hr= _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		CHECK(hr);
	}
}

void Game::CreateInputLayout()
{
	//�Է¿� ���� ���� ~����Ʈ���� �ٸ� Į������ 
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	const int32 count = sizeof(layout)/sizeof(D3D11_INPUT_ELEMENT_DESC);
	//VS������ �Է¿� �����־ vsBlob�� �Ű������� ����
	_device->CreateInputLayout(layout,count, _vsBlob->GetBufferPointer(),_vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}

void Game::CreateVS()
{
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	HRESULT hr = _device->CreateVertexShader(_vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreatePS()
{
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);

	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(),
		_psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(L"cat.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	//���̴����ҽ��� �����
	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
	CHECK(hr);
}

void Game::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;		//CPU Write + GPU Read
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(TransformData);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;		//����� ����ȭ �ǳʶٱ�
	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);

	CHECK(hr);
}
