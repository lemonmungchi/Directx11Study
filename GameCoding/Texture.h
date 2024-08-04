#pragma once

//SRV - 이미지를 어떻게 쓸것인가 - 텍스처
class Texture
{
public:
	Texture(ComPtr<ID3D11Device> device);
	~Texture();

	ComPtr<ID3D11ShaderResourceView> GetComPtr() { return _shaderResourceView; }

	void Create(const wstring& path);

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
};

