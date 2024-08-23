#pragma once
#include "Component.h"
#include "Material.h"
#include "Shader.h"

class Mesh;
class Material;
class Shader;
class Texture;

class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	virtual ~MeshRenderer();

	void SetMaterial(shared_ptr<Material> material) { _material = material; }
	void SetShader(shared_ptr<Shader> shader) { _material->SetShader(shader); }
	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetTexture(shared_ptr<Texture> texture) { _material->SetTexture(texture); }

	auto GetMaterial() { return _material; }
	auto GetVertextShader() { return GetMaterial()->GetShader()->GetVertexShader(); }
	auto GetInputLayout() { return GetMaterial()->GetShader()->GetInputLayout(); }
	auto GetPixelShader() { return GetMaterial()->GetShader()->GetPixelShader(); }


	shared_ptr<Mesh> GetMesh() { return _mesh; }
	shared_ptr<Texture> GetTexture() { return GetMaterial()->GetTexture(); }

private:
	ComPtr<ID3D11Device> _device;

	//이렇게하면 랜더매니저에서 meshRenderer의 변수 protected처럼 쓸수있음
	friend class RenderManager;
	
	//Mesh
	shared_ptr<Mesh> _mesh;
	//Material
	shared_ptr<Material> _material;
};

