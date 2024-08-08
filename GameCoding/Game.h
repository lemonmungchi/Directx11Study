#pragma once


class Game
{
public:
	Game();
	~Game();
public:
	void Init(HWND hwnd);		//윈도우 핸들받아줌
	void Update();
	void Render();
private:

private:
	/// <summary>
	/// 레스터라이저만들기
	/// </summary>
	void CreateRasterizerState();
	/// <summary>
	/// uv관련
	/// </summary>
	void CreateSamplerState();
	void CreateBlenderState();

private:
	HWND _hwnd;

	shared_ptr<Graphics> _graphics;


private:
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
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;

	Vec3 _localposition = { 0.f,0.f,0.f };
	Vec3 _localRotation = { 0.f,0.f,0.f };
	Vec3 _localScale = { 1.f,1.f,1.f };
};

