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
	/// 기하학적인 도형만들기
	/// </summary>
	void CreateGeometry();
	/// <summary>
	/// 입력이 어떻게 이뤄져있는지
	/// </summary>
	void CreateInputLayout();
	//파일에 있던걸 가져와서 어떻게 작동하게 할지 건내줘야해서 가져오는 함수 
	void CreateVS();
	void CreatePS();
	/// <summary>
	/// 레스터라이저만들기
	/// </summary>
	void CreateRasterizerState();
	/// <summary>
	/// uv관련
	/// </summary>
	void CreateSamplerState();
	void CreateBlenderState();
	/// <summary>
	/// 쉐이더 리소스 뷰 
	/// </summary>
	void CreateSRV();
	//상수버퍼 만들기
	void CreateConstantBuffer();

	/// <summary>
	/// 쉐이더 로딩하는 함수 
	/// </summary>
	/// <param name="path"></param>
	/// <param name="name"></param>
	/// <param name="version"></param>
	/// <param name="blob"></param>
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);
private:
	HWND _hwnd;

	Graphics* _graphics;


private:
	//기하학적 도형 - cpu
	vector<Vertex> _vertices;
	VertexBuffer* _vertexBuffer;

	//ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	
	//인덱스버퍼 - 이거도 Geometry에 포함
	vector<uint32> _indices;
	IndexBuffer* _indexBuffer;
	InputLayout* _inputLayout;
	//ComPtr<ID3D11InputLayout> _inputLayout = nullptr;

	//VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;

	//RS
	ComPtr<ID3D11RasterizerState> _rasterizerState = nullptr;

	//PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;

	//SRV - 이미지를 어떻게 쓸것인가
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;

	ComPtr<ID3D11SamplerState> _samplerState = nullptr;
	ComPtr<ID3D11BlendState> _blendState = nullptr;
private:
	//SRT scale, rotate translate
	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer;

	Vec3 _localposition = { 0.f,0.f,0.f };
	Vec3 _localRotation = { 0.f,0.f,0.f };
	Vec3 _localScale = { 1.f,1.f,1.f };
};

