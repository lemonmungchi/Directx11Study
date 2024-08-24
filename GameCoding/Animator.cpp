#include "pch.h"
#include "Animator.h"
#include "Game.h"
#include "TimeManager.h"

Animator::Animator()
	: Super(ComponentType::Animator)
{
}

Animator::~Animator()
{
}

void Animator::Init()
{
}

void Animator::Update()
{
	shared_ptr<Animation> animation = GetCurrentAnimation();
	if (animation == nullptr)
		return;

	const Keyframe& keyframe = animation->GetKeyframe(_currentKeyFrameIndex);

	float deltaTime = TIME->GetDeltaTime();
	_sumTime += deltaTime;

	//하나의 이미지 지속시간이 끝났다면
	if (_sumTime >= keyframe.time)
	{
		//다음 프레임으로
		_currentKeyFrameIndex++;
		int32 totalCount = animation->GetKeyframeCount();

		//다 재생했다면
		if (_currentKeyFrameIndex >= totalCount)
		{
			if (animation->IsLoop())
				_currentKeyFrameIndex = 0;
			else
				_currentKeyFrameIndex = totalCount - 1;
		}

		_sumTime = 0.f;
	}
}

shared_ptr<Animation> Animator::GetCurrentAnimation()
{
	return _currentAnimation;
}

const Keyframe& Animator::GetCurrentKeyframe()
{
	return _currentAnimation->GetKeyframe(_currentKeyFrameIndex);
}
