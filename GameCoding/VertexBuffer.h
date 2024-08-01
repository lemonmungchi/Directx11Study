#pragma once

//지오메트리로 만들어 준 것을 넘겨줘서 사용
class VertexBuffer
{
public:
	VertexBuffer(ComPtr<ID3D11Device> _device);
	~VertexBuffer();

	ComPtr<ID3D11Buffer> GetComPtr() { return _vertexBuffer; }
	uint32 GetStride() { return _stride; }
	uint32 GetOffset() { return _offset; }
	uint32 GetCount() { return _count; }

	template<typename T>
	void Create(const vector<T>& vertices)
	{
		_stride = sizeof(T);		//크기
		_count = static_cast<uint32>(vertices.size());		//정점의 개수


		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;			//gpu만 read only	일회서
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		
		desc.ByteWidth = (uint32)(_stride*_count);

		D3D11_SUBRESOURCE_DATA data; 
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices.data();			//첫번째 시작주소 cpu값이 복사된다.

		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr);
	}

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11Buffer> _vertexBuffer;

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};

