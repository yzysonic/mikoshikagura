#pragma once
#include "Core\Core.h"
#include "Inspector.h"

class DebugManager : public Object, public Singleton<DebugManager>
{
public:
	static void TakeSnapshot(const char* fileName, float waitTime);
	static void OpenObjectExplorer(void);
	static void OpenInspector(Object* object);
	static Inspector* GetInspector(void);
	void Update(void) override;

private:
	friend Singleton<DebugManager>;
	Inspector* inspector;
	DebugManager(void);
};