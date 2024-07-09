#pragma once


class Game
{
public:
	Game();
	~Game();
public:
	void Init(HWND hwnd);		//������ �ڵ�޾���
	void Update();
	void Render();
private:
	/// <summary>
	/// �׸��� �Լ�
	/// </summary>
	void RenderBegin();		
	void RenderEnd();
private:
	void CreateDeviceAndSwapChain();
	/// <summary>
	/// ���۸� ���� Tag�� �޾Ƽ� GPU�� �˷��ֱ� ����
	/// </summary>
	void CreateRenderTargetView();
	/// <summary>
	/// ����Ʈ ���� 
	/// </summary>
	void SetViewport();
//�ﰢ�� �׸�����Ʈ
private:
	/// <summary>
	/// ���������� ���������
	/// </summary>
	void CreateGeometry();
	/// <summary>
	/// �Է��� ��� �̷����ִ���
	/// </summary>
	void CreateInputLayout();
	//���Ͽ� �ִ��� �����ͼ� ��� �۵��ϰ� ���� �ǳ�����ؼ� �������� �Լ� 
	void CreateVS();
	void CreatePS();
	//���̴� ���ҽ� �� 
	void CreateSRV();
	//������� �����
	void CreateConstantBuffer();

	/// <summary>
	/// ���̴� �ε��ϴ� �Լ� 
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
	// I: �������̽� Comptr -> ����Ʈ ������- �ڵ���������, wrl�� �ִ�.
	ComPtr<ID3D11Device> _device; 
	ComPtr<ID3D11DeviceContext> _deviceContext;
	//DXGI : DX�ʹ� ���������� ���� ���� �۾� ���� => �ý���,�ϵ���� ���
	//����ü��-> �׸��� �Ͱ� �����ִ� �� ���� �ؾ� ���� ��� ->���� �ĸ� ��Ӻ���
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	///RTV ����Ÿ�Ϻ�
	ComPtr<ID3D11RenderTargetView> _renderTargetView;

	//Misc 
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.f,0.f,0.f,0.f };
	
	//�ﰢ��

private:
	//�������� ���� - cpu
	vector<Vertex> _vertices; 
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;
	//�ε������� - �̰ŵ� Geometry�� ����
	vector<uint32> _indices;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;

	//VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;
	//PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;

	//SRV - �̹����� ��� �����ΰ�
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;

private:
	//SRT scale, rotate translate
	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer;
};

