#pragma once
#include "Component.h"
class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	virtual ~MeshRenderer();

	virtual void Update() override;
private:
	ComPtr<ID3D11Device> _device;

	//이렇게하면 랜더매니저에서 meshRenderer의 변수 protected처럼 쓸수있음
	friend class RenderManager;
	//Mesh
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	//인덱스버퍼  이거도 Geometry에 포함
	shared_ptr<IndexBuffer> _indexBuffer;

	//Material
	shared_ptr<InputLayout> _inputLayout;
	//VS
	shared_ptr<VertexShader> _vertexShader;
	//PS
	shared_ptr<PixelShader> _pixelShader;
	//SRV - 이미지를 어떻게 쓸것인가 - 텍스처
	shared_ptr<Texture> _texture1;



private:

};

