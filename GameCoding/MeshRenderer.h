#pragma once
#include "Component.h"
class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	virtual ~MeshRenderer();

	virtual void Update() override;
	void Render(shared_ptr<Pipeline> pipeline);
private:
	ComPtr<ID3D11Device> _device;

	//Mesh
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	//인덱스버퍼  이거도 Geometry에 포함
	shared_ptr<IndexBuffer> _indexBuffer;

	//Material
	shared_ptr<InputLayout> _inputLayout;
	//VS
	shared_ptr<VertexShader> _vertexShader;
	//RS
	shared_ptr<RasterizerState> _rasterizerState;
	//PS
	shared_ptr<PixelShader> _pixelShader;
	//SRV - 이미지를 어떻게 쓸것인가 - 텍스처
	shared_ptr<Texture> _texture1;

	//그려주는 파이프라인 상태
	shared_ptr<SamplerState> _samplerState;
	shared_ptr<BlendState> _blendState;

private:
	//Camera
	//쉐이더단계에서 더해줄 수 있는 인자같은 존재
	CameraData _cameraData;
	shared_ptr<ConstantBuffer<CameraData>> _cameraBuffer;

	//SRT scale, rotate translate
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _transformBuffer;
};

