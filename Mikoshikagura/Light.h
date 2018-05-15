#pragma once
#include "Core\Core.h"

#define	NUM_LIGHT (2)	// ���C�g�̐�


class Light : public ILostAndReset
{
public:
	static void Init(void);
	static Light* GetInstance(void);
	HRESULT OnLostDevice(void) override;
	HRESULT OnResetDevice(void) override;

private:
	static D3DLIGHT9 light[NUM_LIGHT];	// ���C�g���
	static smart_ptr<Light> instance;
};
