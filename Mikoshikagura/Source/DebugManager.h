#pragma once
#include "Core\Core.h"
#include "Inspector.h"
#include "FreeCamera.h"

class DebugManager : public Object, public Singleton<DebugManager>
{
public:
	static void TakeSnapshot(const char* fileName, float waitTime);
	static void OpenObjectExplorer(void);
	static void OpenInspector(Object* object);
	static void EnableFreeCamera(void);
	static void DisableFreeCamera(void);
	static FreeCamera* GetFreeCamera(void);
	static Inspector* GetInspector(void);
	void Update(void) override;

private:
	friend Singleton<DebugManager>;
	Inspector* inspector;
	FreeCamera* free_camera;
	Camera* default_camera;
	DebugManager(void);
};