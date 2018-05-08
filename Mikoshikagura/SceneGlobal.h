#pragma once
#include "Core\Core.h"

class SceneGlobal : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
	Object* field;
};