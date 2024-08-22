#include "pch.h"
#include "Shader.h"

ShaderBase::ShaderBase(ComPtr<ID3D11Device> device)
	:_device(device)
{

}

ShaderBase::~ShaderBase()
{
}

/// <summary>
/// 쉐이더 로딩하는 함수 
/// </summary>
/// <param name="path"></param>
/// <param name="name"></param>
/// <param name="version"></param>
/// <param name="blob"></param>
//쉐이더 파일을 건네주면 경로에 따라 로드해서 빌드한 결과물을 블롭에 전달
void ShaderBase::LoadShaderFromFile(const wstring& path, const string& name, const string& version)
{
	_path = path;
	_name = name;

	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;		//디버그 최적화 건너뛰기

	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		_blob.GetAddressOf(),
		nullptr
	);

	CHECK(hr);
}

VertexShader::VertexShader(ComPtr<ID3D11Device> device) : Super(device) 
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Create(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version);
	HRESULT hr = _device->CreateVertexShader(_blob->GetBufferPointer(),
		_blob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

PixelShader::PixelShader(ComPtr<ID3D11Device> device) : Super(device)
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::Create(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version);
	HRESULT hr = _device->CreatePixelShader(_blob->GetBufferPointer(),
		_blob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}
