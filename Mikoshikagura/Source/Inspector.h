	#pragma once
#include "Core\Core.h"
#undef GetObject
#include "Imgui/ImGuiImpl.h"

class InspectorContent
{
public:
	Object* object;
	InspectorContent(void) : object(nullptr) {};
	virtual ~InspectorContent(void) {};
	virtual void Init(void) {};
	virtual void GuiContent(void) = 0;
};

class Inspector : public Script
{
	static constexpr float Width = 330;

public:
	Inspector(void);
	void Init(void) override;
	void Update(void) override;
	void GuiContent(void);
	void SetObject(Object* object);
	Object* GetObject(void);

private:
	InspectorContent* extensionContent;
	std::vector<Script*> script_list;
	ImGuiWindowFlags flags;
	Rigidbody* rigidbody;
	BoxCollider2D* boxcollider2d;
	Object* object;
	bool active;
};

class InspectorExtension : public Script
{
public:
	InspectorExtension(InspectorContent* extensionContent);
	~InspectorExtension(void);
	smart_ptr<InspectorContent> extensionContent;
	void Init(void) override;
};