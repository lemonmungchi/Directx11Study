#include "pch.h"
#include "Game.h"

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
	_gameObject = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
}

void Game::Update()
{
	_gameObject->Update();
}

void Game::Render()
{
	_graphics->RenderBegin();			//초기화

	auto _deviceContext = _graphics->GetDeviceContext();
	// IA - VS - RS - PS -OM
	//TODO : 그리기
	{
		_gameObject->Render(_pipeline); 
	}

	_graphics->RenderEnd();			//제출
}

