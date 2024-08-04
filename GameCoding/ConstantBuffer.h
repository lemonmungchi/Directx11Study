#pragma once

//템플릿클래스는 cpp파일에 구현부를 만들면 안된다!
template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
		:_device(device),_deviceContext(deviceContext)
	{

	}

	~ConstantBuffer(){}

	ComPtr<ID3D11Buffer> GetComPtr() { return _constantBuffer; }

	//상수버퍼 - cpu메모리 -> gpu 복사
	//1단계 버퍼만들기 
	void Create()
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC;		//CPU Write + GPU Read
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//매프레임마다 gpu에 고속복사

		HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
		CHECK(hr);
	}

	void CopyData(const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE subResouce;
		ZeroMemory(&subResouce, sizeof(subResouce));

		//맵을 통해 값을 넣어줄 준비를 한다. 이후 값을 넣고(복사해주고) UNMAP
		_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResouce);
		::memcpy(subResouce.pData, &data, sizeof(data));			//바로 cpu-> gpu 넘어가게 된다.
		_deviceContext->Unmap(_constantBuffer.Get(), 0);
	}
private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	ComPtr<ID3D11Buffer> _constantBuffer;
};

