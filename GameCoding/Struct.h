#pragma once
#include "Types.h"


struct Vertex
{
	Vec3 position;		//12����Ʈ 0���ͽ���
	//Color color;		//12���ͽ���
	Vec2 uv;			
};

struct TransformData
{
	Vec3 offset;
	float dummy;		//������ ���� ����
};
