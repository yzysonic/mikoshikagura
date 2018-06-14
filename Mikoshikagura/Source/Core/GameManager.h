#pragma once

#include "Scene.h"
#include "Singleton.h"
#include "Event.h"
#include <memory>
#include <unordered_map>

#ifdef _DEBUG
#define START_SCENE SceneName::TEST
#else
#define START_SCENE SceneName::TITLE
#endif

class GameManager : public Singleton<GameManager>
{
	friend class Singleton<GameManager>;
	static constexpr UINT SceneStackMax = 3;
public:
	~GameManager(void);

	template<typename T>
	static T& Var(std::string key);	
	static void Update(void);
	static void SetGlobalScene(Scene* scene);
	static void SetScene(Scene* scene);
	static void PushScene(Scene* scene);
	static void PopScene(void);
	static void ReloadScene(void);
	static Scene* GetScene(void);
	static Scene* GetGlobalScene(void);

private:
	GameManager(void);
	std::unique_ptr<Scene> scene[SceneStackMax+2];
	Event set_scene_event;
	UINT scene_stack_num;

	static void SetScene(Scene* scene, int no);
	static void ClearSceneStack(void);
};

template<typename T>
inline T & GameManager::Var(std::string key)
{
	static std::unordered_map<std::string, T> map{};
	return map[key];
}
