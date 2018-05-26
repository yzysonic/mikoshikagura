#include "DebugManager.h"
#include "DebugMenu.h"
#include "Snapshot.h"

DebugManager::DebugManager(void)
{
	AddComponent<DebugMenu>();
	AddComponent<Snapshot>();
}

void DebugManager::Update(void)
{
	if (GetKeyboardTrigger(DIK_F11))
	{
		auto menu = GetComponent<DebugMenu>();
		if (menu->GetActive())
			menu->SetActive(false);
		else
			menu->SetActive(true);
	}
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
