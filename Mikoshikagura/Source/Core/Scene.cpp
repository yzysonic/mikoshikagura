#include "Scene.h"

Scene::~Scene(void)
{
	DestroyAllObject();
}

void Scene::RemoveObject(Object * object)
{
	this->obj_list[object->sceneIndex] = this->obj_list.back();
	this->obj_list[object->sceneIndex]->sceneIndex = object->sceneIndex;
	this->obj_list.pop_back();
}

void Scene::DestroyAllObject(void)
{
	for (auto obj : this->obj_list)
	{
		obj->scene = nullptr;
		obj->Destroy();
	}

	this->obj_list.clear();
}


void Scene::UpdateObjects(void)
{
	for (UINT i = 0; i < this->obj_list.size(); i++)
	{
		auto object = this->obj_list[i];
		if (object->isActive)
		{
			// �I�u�W�F�N�g�̍X�V����
			object->Update();

			// �I�u�W�F�N�g�����̃X�N���v�g�̍X�V����
			for (UINT j = 0; j < object->scripts.size(); j++)
			{
				auto &script = object->scripts[j];
				if (script->GetActive())
					script->Update();
			}
		}
	}

}

void Scene::PauseObjects(void)
{
	if (this->active_shot.size() > 0)
		return;

	for (auto &obj : this->obj_list)
	{
		if (obj->dontStopWhenPause)
			continue;

		this->active_shot.emplace_back(obj->GetActive());
		obj->SetActive(false);
	}
}

void Scene::ResumeObjects(void)
{
	if (this->active_shot.size() == 0)
		return;

	int i = 0;
	for (auto &obj : this->obj_list)
	{
		this->active_shot.emplace_back(obj->GetActive());
		obj->SetActive(this->active_shot[i++]);
	}

	this->active_shot.clear();
}

const std::vector<Object*>& Scene::GetObjectList(void)
{
	return this->obj_list;
}
