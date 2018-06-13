#include "FreeCamera.h"

FreeCamera::FreeCamera(Camera* camera)
{
	// �R���|�[�l���g������
	coordinate	= AddComponent<CameraSphericalCoordinate>();
	smooth		= AddComponent<CameraSmoothFollow>();
	play		= AddComponent<CameraPlay>();

	if (!camera)
		return;

	// �ݒ�̃R�s�[
	at				= camera->at;
	up				= camera->up;
	fov				= camera->fov;
	aspect			= camera->aspect;
	near_z			= camera->near_z;
	far_z			= camera->far_z;
	backColor		= camera->backColor;
	render_target	= camera->render_target;

	auto _coordinate = camera->GetComponent<CameraSphericalCoordinate>();
	if (_coordinate)
	{
		coordinate->phi			= _coordinate->phi;
		coordinate->theta		= _coordinate->theta;
		coordinate->distance	= _coordinate->distance;
		coordinate->SetActive(_coordinate->GetActive());
	}

	auto _smooth = camera->GetComponent<CameraSmoothFollow>();
	if (_smooth)
	{
		smooth->speed		= _smooth->speed;
		smooth->offset_y	= _smooth->offset_y;
		smooth->target		= _smooth->target;
		smooth->SetActive(_smooth->GetActive());
	}

	// �ݒ�𔽉f
	play->SetActive(false);
	play->SetActive(true);
}
