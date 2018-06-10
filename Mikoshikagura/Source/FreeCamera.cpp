#include "FreeCamera.h"

FreeCamera::FreeCamera(Camera* camera)
{
	// コンポーネント初期化
	coordinate	= AddComponent<CameraSphericalCoordinate>();
	smooth		= AddComponent<CameraSmoothFallow>();
	play		= AddComponent<CameraPlay>();

	if (!camera)
		return;

	// 設定のコピー
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

	auto _smooth = camera->GetComponent<CameraSmoothFallow>();
	if (_smooth)
	{
		smooth->speed		= _smooth->speed;
		smooth->offset_y	= _smooth->offset_y;
		smooth->target		= _smooth->target;
		smooth->SetActive(_smooth->GetActive());
	}

	// 設定を反映
	play->SetActive(false);
	play->SetActive(true);
}
