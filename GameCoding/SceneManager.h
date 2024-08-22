#pragma once

class Scene;

class SceneManager
{
public:
	SceneManager(shared_ptr<Graphics> graphics);
	
	void Init();
	void Update();
	
	void LoadScene(wstring sceneName);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }

private:
	shared_ptr<Scene> LoadTestScene();

private:
	//임시적으로 필요한 것 - 그려주는거
	shared_ptr<Graphics> _graphics;
private:
	shared_ptr<Scene> _activeScene;

};

