#pragma once
#include "Types.h"

//
//struct Vertex
//{
//	Vec3 position;		//12바이트 0부터시작
//	//Color color;		//12부터시작
//	Vec2 uv;
//};

struct CameraData
{
	Matrix matView = Matrix::Identity;
	Matrix matProjection= Matrix::Identity;
};

struct TransformData
{
	Matrix matWorld = Matrix::Identity;
};