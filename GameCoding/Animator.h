#pragma once
#include "Component.h"
#include "Animation.h"

class Animation;

class Animator : public Component
{
	using Super = Component;

public:
	Animator();
	virtual ~Animator();

	void Init();
	void Update();

	shared_ptr<Animation> GetCurrentAnimation();
	const Keyframe& GetCurrentKeyframe();

	void SetAnimation(shared_ptr<Animation> animation) { _currentAnimation = animation; }

private:
	float _sumTime = 0.f;

	//현재 실행 중인 키프레임 번호
	int32 _currentKeyFrameIndex = 0;
	shared_ptr<Animation> _currentAnimation;

};

