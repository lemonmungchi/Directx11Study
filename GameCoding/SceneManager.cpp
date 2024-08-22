#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"

SceneManager::SceneManager(shared_ptr<Graphics> graphics)
	:_graphics(graphics)
{

}

void SceneManager::Init()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();

	_activeScene->FixedUpdate();
}

void SceneManager::LoadScene(wstring sceneName)
{
	//리소스를 로드해서 해야하지만 임시로 하드코딩

	_activeScene = LoadTestScene();
	Init();
}


shared_ptr<Scene> SceneManager::LoadTestScene()
{
	//툴이 없기때문에 임의로 씬할당 후 제어
	shared_ptr<Scene> scene = make_shared<Scene>();

	//제어

	//Camera
	shared_ptr<GameObject> camera = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	{
		camera->GetOrAddTransform();
		camera->AddComponent(make_shared<Camera>());
		scene->AddGameObject(camera);
	}

	//cat
	shared_ptr<GameObject> cat = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	{
		cat->GetOrAddTransform();
		cat->AddComponent(make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext()));
		//_cat->GetTransform()->GetScale(Vec3(100.f,100.f,100.f));
		//...
		scene->AddGameObject(cat);
	}
	return scene;
}
