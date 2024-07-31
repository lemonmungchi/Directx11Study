#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(HWND hwnd)
{
	_hwnd = hwnd;

	// 초기화 순서 확인
	_width = GWinSizeX;
	_height = GwinSizeY;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

Graphics::~Graphics()
{
}

void Graphics::RenderBegin()
{
	//여기다가 그릴것이다.
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);				//OM: OUTPUT Mege
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);		//색상으로 초기화해주기
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Graphics::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);			//제출 전면 <= 후면 
	CHECK(hr);
}

void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));		//다 0으로 초기화해줌 필요한 것만 따로 초기화
	{
		desc.BufferDesc.Width = _width;      // 버퍼도 같은 크기로 800 x 600
		desc.BufferDesc.Height = _height;
		desc.BufferDesc.RefreshRate.Numerator = 60;			//화면 주사율
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;			//멀티 샘플링 계단현상, 찌그러짐 어떻게 처리할지 
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			//버퍼를 어떻게 쓸건지 - 그려주는 역활
		desc.BufferCount = 1;		//버퍼몇개
		desc.OutputWindow = _hwnd;		// 결과 윈도우핸들
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}
	//HRESULT 일종의 bool값
	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,			//어떤걸 쓸건지 -> gpu or software로 gpu
		nullptr,
		0,
		nullptr,				//지원하는 dx 버전 레벨 기본값은 내가 지원할 수 있는 상위버전 골라줌 
		0,						//위에 배열 크기
		D3D11_SDK_VERSION,				//매크로
		&desc,
		_swapChain.GetAddressOf(),			//**이면 주소값 가져오는 GetAddressOf()
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	CHECK(hr);
}

void Graphics::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());		//백버퍼 텍스처(PNG)로 가져오기
	CHECK(hr);

	//택스처라는건 그냥 쓰면 아무것도 없는데 거기에 최종 그림을 그리는 곳에 쓸거라는 태그를 달아주는 것 
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());  //렌더타켓뷰라는것으로 텍스처를 명시해 만들어준다.
	CHECK(hr);
}

void Graphics::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}


