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
	_graphics = make_shared<Graphics>(hwnd);
	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());
	_inputLayout = make_shared<InputLayout>(_graphics->GetDevice());
	_geometry = make_shared<Geometry<VertexTextureData>>();
	_vertexShader = make_shared<VertexShader>(_graphics->GetDevice());
	_pixelShader = make_shared<PixelShader>(_graphics->GetDevice());
	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	_texture1 = make_shared<Texture>(_graphics->GetDevice());

	//삼각형 그리기 파트
	/// <summary>
	/// 기하학적인 도형만들기
	/// </summary>
	//정점정보
	GeometryHelper::CreateRectangle(_geometry);

	//정점버퍼
	_vertexBuffer->Create(_geometry->GetVertices());

	//IndexBuffer
	_indexBuffer->Create(_geometry->GetIndices());
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");

	//인풋레이아웃
	/// <summary>
	/// 입력이 어떻게 이뤄져있는지
	/// </summary>
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());

	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");

	CreateRasterizerState();
	CreateSamplerState();
	CreateBlenderState();
	/// <summary>
	/// 쉐이더 리소스 뷰 
	/// </summary>
	_texture1->Create(L"cat.png");

	_constantBuffer->Create();
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

	_constantBuffer->CopyData(_transformData);
}

void Game::Render()
{
	_graphics->RenderBegin();			//초기화

	auto _deviceContext = _graphics->GetDeviceContext();
	// IA - VS - RS - PS -OM
	//TODO : 그리기
	{
		uint32 stride = sizeof(VertexTextureData);
		uint32 offset = 0;
		//IA - 세팅부분
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
		_deviceContext->IASetInputLayout(_inputLayout->GetComPtr().Get());
		_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		//삼각형으로 만들어주기

		//VS
		_deviceContext->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);		//이걸로 일하게 
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer->GetComPtr().GetAddressOf());

		//RS
		_deviceContext->RSSetState(_rasterizerState.Get());

		//PS
		_deviceContext->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _texture1->GetComPtr().GetAddressOf());		//0번슬롯에 1개
		_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());
		//OM
		_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);
		//_deviceContext->Draw(_vertices.size(), 0);
		_deviceContext->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
	}

	_graphics->RenderEnd();			//제출
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
