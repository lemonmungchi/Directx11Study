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
	float _clearColor[4] = { 0.5f,0.5f,0.5f,0.5f };
	
};

