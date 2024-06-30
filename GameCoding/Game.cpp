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


	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	//삼각형 그리기 파트
	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();
}

void Game::Update()
{

}

void Game::Render()
{
	RenderBegin();			//초기화

	// IA - VS - RS - PS -OM
	//TODO : 그리기
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;
		//IA - 세팅부분
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(),&stride, &offset);
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		//삼각형으로 만들어주기

		//VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);		//이걸로 일하게 

		//RS

		//PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		//OM

		_deviceContext->Draw(_vertices.size(), 0);
	}

	RenderEnd();			//제출
}


void Game::RenderBegin()		//랜더링 파이프라인에 우리가 만든거 묶어주기
{
	//여기다가 그릴것이다.
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);				//OM: OUTPUT Mege
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);		//색상으로 초기화해주기
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);			//제출 전면 <= 후면 
	CHECK(hr);
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));		//다 0으로 초기화해줌 필요한 것만 따로 초기화
	{
		desc.BufferDesc.Width = _width;		//버퍼도 같은 크기로 800 x 600
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

void Game::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());		//백버퍼 텍스처(PNG)로 가져오기
	CHECK(hr);

	//택스처라는건 그냥 쓰면 아무것도 없는데 거기에 최종 그림을 그리는 곳에 쓸거라는 태그를 달아주는 것 
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());  //렌더타켓뷰라는것으로 텍스처를 명시해 만들어준다.
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

void Game::CreateGeometry()
{
	//정점정보
	{
		_vertices.resize(3);

		_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f);
		_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);
		_vertices[1].position = Vec3(0.f, 0.5f, 0.f);
		_vertices[1].color = Color(0.f, 1.f, 0.f, 1.f);
		_vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
		_vertices[2].color = Color(0.f, 0.f, 1.f, 1.f);
	}

	//정점버퍼
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;			//gpu만 read only
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();			//첫번째 시작주소 cpu값이 복사된다.

		_device->CreateBuffer(&desc,&data, _vertexBuffer.GetAddressOf());
	}
	
}

void Game::CreateInputLayout()
{
	//입력에 대한 정보 ~바이트부터 뛰면 칼러인지 
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	const int32 count = sizeof(layout)/sizeof(D3D11_INPUT_ELEMENT_DESC);
	//VS과정의 입력에 관련있어서 vsBlob을 매개변수로 해줌
	_device->CreateInputLayout(layout,count, _vsBlob->GetBufferPointer(),_vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}

void Game::CreateVS()
{
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	HRESULT hr = _device->CreateVertexShader(_vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreatePS()
{
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);

	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(),
		_psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;		//디버그 최적화 건너뛰기
	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);

	CHECK(hr);
}
