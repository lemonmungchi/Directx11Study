#include "pch.h"
#include "Game.h"
#include "Camera.h"
#include "MeshRenderer.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;

	_graphics = make_shared<Graphics>(hwnd);
	_pipeline = make_shared<Pipeline>(_graphics->GetDeviceContext());

	//GO
	_cat = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	{
		_cat->GetOrAddTransform();
		_cat->AddComponent(make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext()));
		//_cat->GetTransform()->GetScale(Vec3(100.f,100.f,100.f));
		//...
	}
	_camera = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	{
		_camera->GetOrAddTransform();
		_camera->AddComponent(make_shared<Camera>());
	}
}

void Game::Update()
{
	_cat->Update();



	_camera->Update();
}

void Game::Render()
{
	_graphics->RenderBegin();			//초기화

	auto _deviceContext = _graphics->GetDeviceContext();
	// IA - VS - RS - PS -OM
	//TODO : 그리기
	{
		//TMP
		_cat->GetMeshRenderer()->Render(_pipeline);
	}

	_graphics->RenderEnd();			//제출
}

