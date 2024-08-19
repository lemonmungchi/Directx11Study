#pragma once
#include "Component.h"

enum class ProjectType
{
	Persperctive,		//원근투영
	Orthographic,		//직교투영
};

class Camera : public Component
{
	using Super = Component;
public:
	Camera();
	virtual ~Camera();

	virtual void Update() override;

	void SetProjectionType(ProjectType type) { _type = type; }
	ProjectType GetProjectionType() { return _type; }

	void UpdateMatrix();

private:
	ProjectType _type = ProjectType::Orthographic;

public:
	//일단 카메라가 한개라고 가정
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};

