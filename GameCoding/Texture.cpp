#include "pch.h"
#include "Texture.h"

Texture::Texture(ComPtr<ID3D11Device> device) 
	:_device(device)
{
}

Texture::~Texture()
{
}

void Texture::Create(const wstring& path)
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	//쉐이더리소스뷰 만들기
	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
	CHECK(hr);
}
