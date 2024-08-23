#pragma once
#include "ResourceBase.h"
class Shader :  public ResourceBase
{
	using Super = ResourceBase;
public:
	Shader();
	virtual ~Shader();

	shared_ptr<InputLayout> GetInputLayout() { return _inputLayout; }
	shared_ptr<VertexShader> GetVertexShader() { return _vertexShader; }
	shared_ptr<PixelShader> GetPixelShader() { return _pixelShader; }

private:
	friend class ResourceManager;
	//Material
	//쉐이더 넘겨줄 때 어떤모양으로 되어있는지
	shared_ptr<InputLayout> _inputLayout;
	//VS
	shared_ptr<VertexShader> _vertexShader;
	//PS
	shared_ptr<PixelShader> _pixelShader;
	//SRV - 이미지를 어떻게 쓸것인가 - 텍스처
};

