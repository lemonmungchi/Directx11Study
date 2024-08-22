#pragma once

enum class ResourceType : uint8
{
	None = -1,
	Mesh,
	Shader,
	Texture,
	Material,
	Animation,

	End		//카운팅하기 위해
};

enum
{
	RESOURCE_TYPE_COUNT = static_cast<uint8>(ResourceType::End)
};

class ResourceBase : public enable_shared_from_this<ResourceBase>
{
public:
	ResourceBase(ResourceType type);
	virtual ~ResourceBase();

	ResourceType GetType() { return _type; }

	void SetName(const wstring& name) { _name = name; }
	const wstring& GetName() { return _name; }
	uint32 GetID() { return _id; }
 
protected:
	//파일 입출력
	virtual void Load(const wstring& path) { }
	virtual void Save(const wstring& path) { }

protected:
	ResourceType _type = ResourceType::None;
	wstring _name;
	wstring _path;
	uint32 _id = 0;
};

