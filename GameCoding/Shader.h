#pragma once

//쉐이더에서 리소스를 사용할 때 어디서사용할지 지칭하는
enum ShaderScope
{
	//비트 플래그용도
	SS_None = 0,
	SS_VertexShader = (1 << 0),
	SS_PixelShader = (1 << 1),
	SS_Both= SS_VertexShader | SS_PixelShader,
};

//쉐이더 공용부
class Shader
{
public:
	Shader(ComPtr<ID3D11Device> device);
	virtual ~Shader();

	virtual void Create(const wstring& path, const string& name, const string& version) abstract;

	ComPtr<ID3DBlob> GetBlob() { return _blob; }

protected:
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version);
protected:
	wstring _path;
	string _name;
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3DBlob> _blob = nullptr;
};

class VertexShader : public Shader
{
	//부모클래스지정
	using Super = Shader;
public:
	VertexShader(ComPtr<ID3D11Device> device);
	~VertexShader();
	//VS
	ComPtr<ID3D11VertexShader> GetComPtr() { return _vertexShader; }

	virtual void Create(const wstring& path, const string& name, const string& version) override;

protected:
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
};

class PixelShader : public Shader
{
	//부모클래스지정
	using Super = Shader;
public:
	PixelShader(ComPtr<ID3D11Device> device);
	~PixelShader();
	//PS
	ComPtr<ID3D11PixelShader> GetComPtr() { return _pixelShader; }

	virtual void Create(const wstring& path, const string& name, const string& version) override;

protected:
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
};

