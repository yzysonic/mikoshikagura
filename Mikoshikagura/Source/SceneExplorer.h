#pragma once

#include "Core\Core.h"
#include "Imgui/ImGuiImpl.h"

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
	
	std::vector<smart_ptr<SceneDataBase>> sceneList;
	ImGuiWindowFlags windowFlags;

	// �����o�[�֐�
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
	name = TypeName<T>();
	std::string texName = "Snapshot_" + name;
	if (Texture::Load(texName))
		texture = Texture::Get(texName);
	else
		texture = Texture::none;
}

template<class T>
inline void SceneExplorer::SceneData<T>::SetScene(void)
{
	GameManager::GetInstance()->SetScene(new T);
}