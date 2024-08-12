#include "pch.h"
#include "Transform.h"

Transform::Transform()
{
}

Transform::~Transform()
{
}

void Transform::Init()
{
}

void Transform::Update()
{
}

void Transform::UpdateTransform()
{
	//크기 회전 이동
	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localposition);

	//로컬좌표에서의 SRT변환행렬
	_matLocal = matScale * matRotation * matTranslation;
	
	if (HasParent())
	{
		_matWorld = _matLocal * _parent->GetWorldMatrix();
	}
	else
	{
		//부모가 없을때는 로컬좌표가 월드좌표
		_matWorld = _matLocal;
	}

}