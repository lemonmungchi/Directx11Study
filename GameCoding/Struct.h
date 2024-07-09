#pragma once
#include "Types.h"


struct Vertex
{
	Vec3 position;		//12바이트 0부터시작
	//Color color;		//12부터시작
	Vec2 uv;			
};

struct TransformData
{
	Vec3 offset;
	float dummy;		//정렬을 위한 더미
};
