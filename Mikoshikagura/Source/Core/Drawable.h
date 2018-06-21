#pragma once

#include "Common.h"
#include "Component.h"
#include "Layer.h"
#include "RendererType.h"
#include "Color.h"
#include "Shader.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

class Drawable : public Component
{
	friend class RenderSpace;

public:
	RendererType rendType;	// �`����@�w��
	VertexShader* vshader;
	PixelShader* pshader;

	Drawable(Layer layer, std::string render_space_name);
	virtual ~Drawable(void);
	virtual void Draw(void) = 0;
	virtual void SetColor(Color color);

	bool SetActive(bool value) override;
	void SetRenderSpace(std::string render_space);
	Layer GetLayer(void);
	void SetLayer(Layer layer);
	Color GetColor(void);
	float GetOpacity(void);
	void SetOpacity(float opacity);

protected:
	Layer layer;	// �`��̃��C���[
	Color color;	// �F
	std::string render_space;
	int list_index;
	int render_space_index;


};
