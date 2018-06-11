#include "CameraSphericalCoordinate.h"

void CameraSphericalCoordinate::Init(void)
{
	camera = dynamic_cast<Camera*>(object);

	auto offset = camera->transform.position - camera->at;

	theta = atan2f(sqrtf(offset.x*offset.x + offset.z*offset.z), offset.y);
	phi = atan2f(offset.z, offset.x);
	distance = offset.length();

}

void CameraSphericalCoordinate::Update(void)
{
	camera->transform.position.y = distance * cosf(theta);
	camera->transform.position.x = distance * sinf(theta) * cosf(phi);
	camera->transform.position.z = distance * sinf(theta) * sinf(phi);
	camera->transform.position += camera->at;
}
