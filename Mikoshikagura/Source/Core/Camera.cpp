#include "Camera.h"
#include "Renderer.h"
#include "Math.h"

#ifdef _DEBUG
#include "../InspectorContentCamera.h"
#endif

Camera::Camera(RenderTarget* render_target)
{
	if (render_target == nullptr)
		this->render_target = RenderTarget::BackBuffer();
	else this->render_target = render_target;
	this->name = "Camera";
	this->transform.position = Vector3(0.0f, 0.0f, -150.0f);
	this->setBackColor(Color(255, 255, 255, 255));
	this->at = Vector3(0.0f, 0.0f, 1.0f);
	this->up = Vector3(0.0f, 1.0f, 0.0f);
	this->aspect = (float)SystemParameters::ResolutionX / (float)SystemParameters::ResolutionY;
	this->near_z = 10.0f;
	this->far_z = 1000.0f;
	this->fov = Deg2Rad(60.0f);

#ifdef _DEBUG
	this->AddComponent<InspectorExtension>(new InspectorContentCamera);
#endif
}

D3DXMATRIX Camera::getViewMatrix(bool update)
{
	if (update)
	{
		// �r���[�}�g���N�X�̏�����
		D3DXMatrixIdentity(&view);

		// �r���[�}�g���N�X�̍쐬
		D3DXVECTOR3 eye = this->transform.position;
		D3DXVECTOR3 at = this->at;
		D3DXVECTOR3 up = this->up;
		D3DXMatrixLookAtLH(&view, &eye, &at, &up);
	}

	return view;
}

D3DXMATRIX Camera::getProjectionMatrix(bool update)
{
	if (update)
	{
		// �v���W�F�N�V�����}�g���N�X�̏�����
		D3DXMatrixIdentity(&projection);

		// �v���W�F�N�V�����}�g���N�X�̍쐬
		D3DXMatrixPerspectiveFovLH(&projection, fov, aspect, near_z, far_z);
	}

	return projection;
}

void Camera::setBackColor(Color color)
{
	backColor = ColorRGBA(color.r, color.g, color.b, color.a);
}

void Camera::setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	backColor = ColorRGBA(r, g, b, a);
}
