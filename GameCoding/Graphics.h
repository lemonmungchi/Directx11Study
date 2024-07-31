#pragma once
class Graphics
{
public:
	Graphics(HWND hwnd);
	~Graphics();

	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

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
};

