#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "Pipeline.h"

MeshRenderer::MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: Super(ComponentType::MeshRenderer), _device(device)
{
	//정점정보 - 사각형 만들기
	_geometry = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateRectangle(_geometry);

	//정점버퍼
	_vertexBuffer = make_shared<VertexBuffer>(device);
	_vertexBuffer->Create(_geometry->GetVertices());

	//IndexBuffer
	_indexBuffer = make_shared<IndexBuffer>(device);
	_indexBuffer->Create(_geometry->GetIndices());

	_vertexShader = make_shared<VertexShader>(device);
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");

	//인풋레이아웃
	/// <summary>
	/// 입력이 어떻게 이뤄져있는지
	/// </summary>
	_inputLayout = make_shared<InputLayout>(device);
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());

	_pixelShader = make_shared<PixelShader>(device);
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");

	_texture1 = make_shared<Texture>(device);
	_texture1->Create(L"cat.png");

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Update()
{

}
