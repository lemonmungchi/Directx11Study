#pragma once
#include "ResourceBase.h"

struct Keyframe
{
	//어디서 부터 그릴 것인지
	Vec2 offset = Vec2{ 0.f,0.f };
	//영역의 크기
	Vec2 size = Vec2{ 0.f,0.f };
	//몇초동안 그릴 것인지
	float time = 0.f;
};

class Animation : public ResourceBase
{
	using Super = ResourceBase;
public:
	Animation();
	virtual ~Animation();

	virtual void Load(const wstring& path) override;
	virtual void Save(const wstring& path) override;

	void SetLoop(bool loop) { _loop = loop; }
	bool IsLoop() { return _loop; }

	void SetTexture(shared_ptr<Texture> texture) { _texture = texture; }
	shared_ptr<Texture> GetTexture() { return _texture; }
	Vec2 GetTextureSize();

	const Keyframe& GetKeyframe(int32 index);
	int32 GetKeyframeCount();
	//원하는 애니메이션 정보 추가
	void AddKeyframe(const Keyframe& keyframe);

private:
	bool _loop = false;
	shared_ptr<Texture> _texture;
	vector<Keyframe> _keyframes;
};

