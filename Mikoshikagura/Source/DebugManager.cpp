#include "DebugManager.h"
#include "Snapshot.h"
#include "ObjectExplorer.h"

#ifdef IMGUI
#include "DebugMenu.h"
#endif

DebugManager::DebugManager(void)
{
#ifdef IMGUI
	AddComponent<DebugMenu>();
#endif
	AddComponent<Snapshot>();
	AddComponent<ObjectExplorer>();
	inspector = AddComponent<Inspector>();
}

void DebugManager::Update(void)
{
#ifdef IMGUI
	if (GetKeyboardTrigger(DIK_F11))
	{
		auto menu = GetComponent<DebugMenu>();
		if (menu->GetActive())
			menu->SetActive(false);
		else
			menu->SetActive(true);
	}
#endif
}

void DebugManager::TakeSnapshot(const char * fileName, float waitTime)
{
	auto snapshot = m_pInstance->GetComponent<Snapshot>();

	if (snapshot == nullptr)
		return;
	if (snapshot->GetActive())
		return;

	snapshot->timer.Reset(waitTime);
	snapshot->fileName = fileName;
	snapshot->SetActive(true);

}

void DebugManager::OpenObjectExplorer(void)
{
	m_pInstance->GetComponent<ObjectExplorer>()->SetActive(true);
}

void DebugManager::OpenInspector(Object * object)
{
	m_pInstance->inspector->SetObject(object);
	m_pInstance->inspector->SetActive(true);
}

Inspector* DebugManager::GetInspector(void)
{
	return m_pInstance->inspector;
}
