//#include "Sunlight.h"
//
//
//Sunlight::Sunlight()
//{
//	rayspeed = 1.0f;
//	raddelta = D3DXToRadian(1.1f);
//	radius = 0;
//}
//
//void Sunlight::SetRay(int raynum)
//{
//	for (int i = 0; i < raynum; i++)
//	{
//		Object* objtemp = new Object;
//		objtemp->AddComponent<RectPolygon2D>("sunlight_clear");
//		objtemp->GetComponent<RectPolygon2D>()->SetSize(300.0f,(float) SystemParameters::ResolutionY);
//
//		objtemp->transform.position.x = -SystemParameters::ResolutionX/2+100;
//		objtemp->transform.position.y = 0.0f;
//		objtemp->transform.setRotation(0.0f,0.0f, 0.0f);
//		raylist.push_back(objtemp);
//	}
//}
//
//void Sunlight::Update()
//{
//	float temp;
//	radius += raddelta;
//	temp = sinf(radius);
//	Vector3 vectemp;
//
//	for (auto itr : raylist) {
//		//itr->transform.position.x += temp * this->rayspeed;
//		vectemp = itr->transform.position;
//		vectemp.x += temp;
//		itr->transform.setRotation(vectemp);
//	}
//
//}
