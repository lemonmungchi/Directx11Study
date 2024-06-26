#pragma once


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
	uint32 _width = 0;
	uint32 _height = 0;

private:
	//DX
};

