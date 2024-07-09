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
	/// <summary>
	/// 그리기 함수
	/// </summary>
	void RenderBegin();		
	void RenderEnd();
private:
	void CreateDeviceAndSwapChain();
	/// <summary>
	/// 버퍼를 묘사 Tag를 달아서 GPU에 알려주기 위함
	/// </summary>
	void CreateRenderTargetView();
	/// <summary>
	/// 뷰포트 묘사 
	/// </summary>
	void SetViewport();
//삼각형 그리기파트
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
	//쉐이더 리소스 뷰 
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
	uint32 _width = 0;
	uint32 _height = 0;

private:
	//Device & SwapChain
	// I: 인터페이스 Comptr -> 스마트 포인터- 자동관리해줌, wrl에 있다.
	ComPtr<ID3D11Device> _device; 
	ComPtr<ID3D11DeviceContext> _deviceContext;
	//DXGI : DX와는 독립적으로 하위 수준 작업 관리 => 시스템,하드웨어 통신
	//스왑체인-> 그리는 것과 보여주는 것 따로 해야 보통 방식 ->전면 후면 고속복사
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	///RTV 렌더타켓뷰
	ComPtr<ID3D11RenderTargetView> _renderTargetView;

	//Misc 
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.f,0.f,0.f,0.f };
	
	//삼각형

private:
	//기하학적 도형 - cpu
	vector<Vertex> _vertices; 
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;
	//인덱스버퍼 - 이거도 Geometry에 포함
	vector<uint32> _indices;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;

	//VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;
	//PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;

	//SRV - 이미지를 어떻게 쓸것인가
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;

private:
	//SRT scale, rotate translate
	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer;
};

