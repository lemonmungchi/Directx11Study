#pragma once
class GameObject
{
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~GameObject();

	void Update();
	void Render(shared_ptr<Pipeline> pipeline);
private:
	ComPtr<ID3D11Device> _device;

	//기하학적 도형 - cpu
	//vector<Vertex> _vertices;
	//vector<uint32> _indices;

	shared_ptr<Geometry<VertexTextureData>> _geometry;

	shared_ptr<VertexBuffer> _vertexBuffer;

	//인덱스버퍼 - 이거도 Geometry에 포함
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	//VS
	shared_ptr<VertexShader> _vertexShader;

	//RS
	shared_ptr<RasterizerState> _rasterizerState;

	//PS
	shared_ptr<PixelShader> _pixelShader;

	//SRV - 이미지를 어떻게 쓸것인가 - 텍스처
	shared_ptr<Texture> _texture1;

	shared_ptr<SamplerState> _samplerState;
	shared_ptr<BlendState> _blendState;
private:
	//SRT scale, rotate translate
	//쉐이더단계에서 더해줄 수 있는 인자같은 존재
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;

	shared_ptr<Transform> _transform = make_shared<Transform>();

	shared_ptr<Transform> _parent = make_shared<Transform>();
};

