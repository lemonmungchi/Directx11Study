#pragma once
#include "Graphics.h"
#include "GameObject.h"

class Game
{
public:
	Game();
	~Game();
public:
	void Init(HWND hwnd);		//윈도우 핸들받아줌
	void Update();
	void Render();
private:
	HWND _hwnd;

	shared_ptr<Graphics> _graphics;
	shared_ptr<Pipeline> _pipeline;

	shared_ptr<GameObject> _cat;
	shared_ptr<GameObject> _camera;
};