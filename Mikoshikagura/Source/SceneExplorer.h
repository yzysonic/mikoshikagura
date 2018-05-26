#pragma once
#include "Core\Core.h"

class SceneExplorer : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	// 表示用シーンデータ（ベース）
	class SceneDataBase
	{
	public:
		std::string name;
		Texture* texture;
		virtual void SetScene(void) = 0;
	};

	// 表示用シーンデータ
	template<class T>
	class SceneData : public SceneDataBase
	{
	public:
		SceneData(void);
		void SetScene(void) override;
	};

	// メンバー変数
	int columns;
	bool isAutoSnapshot;
	
	std::vector<smart_ptr<SceneDataBase>> sceneList;
	ImGuiWindowFlags windowFlags;

	// メンバー関数
	template<class T>
	void AddScene(void);

};

template<class T>
inline void SceneExplorer::AddScene(void)
{
	static_assert(std::is_base_of<Scene, T>::value, "Not a subclass of Scene.");

	sceneList.push_back(std::make_unique<SceneData<T>>());
}

template<class T>
inline SceneExplorer::SceneData<T>::SceneData(void)
{
	name = typeid(T).name();
	name = name.substr(6, (name.size() - 6));

	std::string texName = "Snapshot_" + name;
	if (Texture::LoadTexture(texName))
		texture = Texture::Get(texName);
	else
		texture = Texture::none;
}

template<class T>
inline void SceneExplorer::SceneData<T>::SetScene(void)
{
	GameManager::GetInstance()->SetScene(new T);
}
