#include "pch.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_width = GWinSizeX;
	_height = GwinSizeY;

	//TODO
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

void Game::Update()
{

}

void Game::Render()
{
	RenderBegin();			//�ʱ�ȭ

	//TODO : �׸���


	RenderEnd();			//����
}


void Game::RenderBegin()		//������ ���������ο� �츮�� ����� �����ֱ�
{
	//����ٰ� �׸����̴�.
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);				//OM: OUTPUT Mege
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);		//�������� �ʱ�ȭ���ֱ�
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);			//���� ���� <= �ĸ� 
	CHECK(hr);
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));		//�� 0���� �ʱ�ȭ���� �ʿ��� �͸� ���� �ʱ�ȭ
	{
		desc.BufferDesc.Width = _width;		//���۵� ���� ũ��� 800 x 600
		desc.BufferDesc.Height = _height;
		desc.BufferDesc.RefreshRate.Numerator = 60;			//ȭ�� �ֻ���
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;			//��Ƽ ���ø� �������, ��׷��� ��� ó������ 
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			//���۸� ��� ������ - �׷��ִ� ��Ȱ
		desc.BufferCount = 1;		//���۸
		desc.OutputWindow = _hwnd;		// ��� �������ڵ�
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}
	//HRESULT ������ bool��
	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,			//��� ������ -> gpu or software�� gpu
		nullptr,
		0,
		nullptr,				//�����ϴ� dx ���� ���� �⺻���� ���� ������ �� �ִ� �������� ����� 
		0,						//���� �迭 ũ��
		D3D11_SDK_VERSION,				//��ũ��
		&desc,
		_swapChain.GetAddressOf(),			//**�̸� �ּҰ� �������� GetAddressOf()
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	CHECK(hr);
}

void Game::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());		//����� �ؽ�ó(PNG)�� ��������
	CHECK(hr);

	//�ý�ó��°� �׳� ���� �ƹ��͵� ���µ� �ű⿡ ���� �׸��� �׸��� ���� ���Ŷ�� �±׸� �޾��ִ� �� 
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());  //����Ÿ�Ϻ��°����� �ؽ�ó�� ����� ������ش�.
	CHECK(hr);
	
}

void Game::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MaxDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}
