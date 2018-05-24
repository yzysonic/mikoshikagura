#pragma once
#include "Core\Core.h"

class SceneExplorer : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	// �\���p�V�[���f�[�^�i�x�[�X�j
	class SceneDataBase
	{
	public:
		std::string name;
		Texture* texture;
		virtual void SetScene(void) = 0;
	};

	// �\���p�V�[���f�[�^
	template<class T>
	class SceneData : public SceneDataBase
	{
	public:
		SceneData(void);
		void SetScene(void) override;
	};

	// �����o�[�ϐ�
	int columns;
	bool isAutoSnapshot;
	std::vector<SceneDataBase*> sceneList;
	ImGuiWindowFlags windowFlags;

	// �����o�[�֐�
	template<class T>
	void AddScene(void);

};

template<class T>
inline void SceneExplorer::AddScene(void)
{
	static_assert(std::is_base_of<Scene, T>::value, "Not a subclass of Scene.");

	sceneList.push_back(new SceneData<T>);
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