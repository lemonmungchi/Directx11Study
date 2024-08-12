#pragma once
#include "Component.h"

class Transform: public Component
{
public:
	Transform();
	~Transform();


	virtual void  Init() override;
	virtual void Update() override;

	void UpdateTransform();

	//Local
	Vec3 GetLocalScale() { return _localScale; }
	void SetLocalScale(const Vec3& localScale) { _localScale = localScale; UpdateTransform(); }
	Vec3 GetLocalRotation() { return _localRotation; }
	void SetLocalRotation(const Vec3& localRotation) { _localRotation = localRotation; UpdateTransform();}
	Vec3 GetLocalPosition() { return _localposition; }
	void SetLocalPosition(const Vec3& localPosition) { _localposition = localPosition; UpdateTransform();}

	
	//World
	Vec3 GetScale() { return _scale; }
	void SetScale(const Vec3& worldScale);
	Vec3 GetRotation() { return _rotation; }
	void SetRotation(const Vec3& worldRotation);
	Vec3 GetPosition() { return _position; }
	void SetPosition(const Vec3& worldPosition);

	Matrix GetWorldMatrix() { return _matWorld; }

	//계층관계
	bool HasParent() { return _parent != nullptr; }
	shared_ptr<Transform> GetParent() { return _parent; }
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }

	const vector<shared_ptr<Transform>>& GetChildren() { return _children; }
	void AddChild(shared_ptr<Transform> child) { _children.push_back(child); }


private:

	//SRT
	Vec3 _localScale = { 1.f,1.f,1.f };
	Vec3 _localRotation = { 0.f,0.f,0.f };
	Vec3 _localposition = { 0.f,0.f,0.f };
	

	//Cache
	//항등행렬
	Matrix _matLocal = Matrix::Identity;
	Matrix _matWorld = Matrix::Identity;

	Vec3 _scale;
	Vec3 _rotation;
	Vec3 _position;

	Vec3 _right;
	Vec3 _up;
	Vec3 _look;

private:
	//부모 오브젝트
	shared_ptr<Transform> _parent;
	//자식오브젝트
	vector<shared_ptr<Transform>> _children;
};

