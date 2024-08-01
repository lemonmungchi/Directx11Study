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

	//_graphics=make_shared<Graphics>(hwnd):
	_graphics = new Graphics(hwnd);
	_vertexBuffer = new VertexBuffer(_graphics->GetDevice());
	_indexBuffer = new IndexBuffer(_graphics->GetDevice());
	_inputLayout = new InputLayout(_graphics->GetDevice());
	//삼각형 그리기 파트
	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();


	CreateRasterizerState();
	CreateSamplerState();
	CreateBlenderState();

	CreateSRV();
	CreateConstantBuffer();
}

void Game::Update()
{
	//크기 회전 이동
	Matrix matScale = Matrix::CreateScale(_localScale/3);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localposition);

	Matrix matWorld = matScale * matRotation * matTranslation;	//SRT
	_transformData.matWorld = matWorld;

	D3D11_MAPPED_SUBRESOURCE subResouce;
	ZeroMemory(&subResouce, sizeof(subResouce));

	//맵을 통해 값을 넣어줄 준비를 한다. 이후 값을 넣고(복사해주고) UNMAP
	_graphics->GetDeviceContext()->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResouce);
	::memcpy(subResouce.pData, &_transformData, sizeof(_transformData));			//바로 cpu-> gpu 넘어가게 된다.
	_graphics->GetDeviceContext()->Unmap(_constantBuffer.Get(), 0);
}

void Game::Render()
{
	_graphics->RenderBegin();			//초기화

	auto _deviceContext = _graphics->GetDeviceContext();
	// IA - VS - RS - PS -OM
	//TODO : 그리기
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;
		//IA - 세팅부분
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
		_deviceContext->IASetInputLayout(_inputLayout->GetComPtr().Get());
		_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		//삼각형으로 만들어주기

		//VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);		//이걸로 일하게 
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

		//RS
		_deviceContext->RSSetState(_rasterizerState.Get());

		//PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());		//0번슬롯에 1개
		_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());
		//OM
		_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);
		//_deviceContext->Draw(_vertices.size(), 0);
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

	_graphics->RenderEnd();			//제출
}


	void Game::CreateGeometry()
	{
		//정점정보
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

		//정점버퍼
		{
			_vertexBuffer->Create(_vertices);
		}

	//Index
	{
		//중복되는 점의 사용을 줄이려고 
		_indices = { 0,1,2,2,1,3 };
	}

	//IndexBuffer
	{
		_indexBuffer->Create(_indices);
	}
}

void Game::CreateInputLayout()
{
	//입력에 대한 정보 ~바이트부터 뛰면 칼러인지 
	vector<D3D11_INPUT_ELEMENT_DESC> layout
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	_inputLayout->Create(layout,_vsBlob);
	
}

void Game::CreateVS()
{
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	HRESULT hr = _graphics->GetDevice()->CreateVertexShader(_vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreatePS()
{
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);

	HRESULT hr = _graphics->GetDevice()->CreatePixelShader(_psBlob->GetBufferPointer(),
		_psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	//기본 솔리드 / 백
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;

	HRESULT hr = _graphics->GetDevice()->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	//순서대로 RGBA
	desc.BorderColor[0] = 1;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 16;
	desc.MaxLOD = FLT_MAX;
	desc.MinLOD = FLT_MIN;
	desc.MipLODBias = 0.0f;

	_graphics->GetDevice()->CreateSamplerState(&desc, _samplerState.GetAddressOf());
}

void Game::CreateBlenderState()
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = _graphics->GetDevice()->CreateBlendState(&desc, _blendState.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(L"cat.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	//쉐이더리소스뷰 만들기
	hr = ::CreateShaderResourceView(_graphics->GetDevice().Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
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

	HRESULT hr = _graphics->GetDevice()->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;		//디버그 최적화 건너뛰기
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
