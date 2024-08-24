#pragma once

struct CameraData
{
	Matrix matView = Matrix::Identity;
	Matrix matProjection = Matrix::Identity;
};

struct TransformData
{
	Matrix matWorld = Matrix::Identity;
};

//GPU로 넘겨줄때 16바이트로 정렬
struct AnimationData
{
	Vec2 spriteOffset;
	Vec2 spriteSize;
	//전체 사이즈
	Vec2 textureSize;
	float useAnimation;
	//16바이트 채우기위한 더미
	float padding;
};

