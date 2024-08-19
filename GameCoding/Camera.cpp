#include "pch.h"
#include "Camera.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera() : Super(ComponentType::Camera)
{

}

Camera::~Camera()
{

}

void Camera::Update()
{
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	//카메라좌표
	Vec3 eyePosition = GetTransform()->GetPosition();
	//바라보는 방향
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook(); 
	//위쪽방향 - 임의로 정해줘도 된다.
	Vec3 upDirection = GetTransform()->GetUp();
	//카메라 변환행렬 계산 - 월드의 역행렬
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);

	//방법2 - 월드의 역행렬
	//S_MatView = GetTransform()->GetWorldMatrix().Invert();

	if (_type == ProjectType::Persperctive)
	{
		//필드오브 뷰,비율,니어,파
		S_MatProjection = ::XMMatrixPerspectiveFovLH(XM_PI / 4.f, 800.f / 600.f, 1.f, 100.f);
	}
	else
	{
		//화면의 크기, 니어,파
		S_MatProjection = ::XMMatrixOrthographicLH(8, 6, 0.f, 1.f);
	}
}
