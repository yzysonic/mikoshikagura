#include "XMLPerser.h"

Mapdata::Mapdata(){
} 

Mapdata::Mapdata(std::string str)
{
	std::ifstream ifs(str);
	maplayer mapbaff;


	bool flag = false;

	if (ifs.fail()) {
		std::cerr << "Failed to open file." << std::endl;
		return ;
	}


	while (std::getline(ifs, str)) {
		if (str.find("data")  !=  -1  ) {
			if (flag) {
				flag = false;

				layer.push_back(mapbaff);
				mapbaff.maptip.clear();
			}else {
				flag = true;
			}
		}
		else {
			if (flag == true) {

				std::stringstream ss(str);
				std::string tipbuff;
				std::vector<int> xbuff;

				while (std::getline(ss, tipbuff, ',')) {
					xbuff.push_back(std::stoi(tipbuff));
				}

				mapbaff.maptip.push_back(xbuff);

			}


		}

	}



	layermax = layer.size();
	height = layer[0].maptip.size();
	width = layer[0].maptip[0].size();

}

void Mapdata::Load(std::string str)
{
	std::ifstream ifs(str);

	if (ifs.fail()) {
		std::cerr << "Failed to open file." << std::endl;

	}


	while (std::getline(ifs, str)) {
		std::cout << "#" << str << std::endl;


	}

}

void Mapdata::MapView()
{
	int i=0;
	for (auto itr = layer.begin(); itr != layer.end(); ++itr) {
		std::cout << i << std::endl;
		for (auto itr2 = itr->maptip.begin(); itr2 != itr->maptip.end(); ++itr2) {

			for (auto itr3 = itr2->begin(); itr3 != itr2->end(); ++itr3) {

				std::cout << *(itr3);
			}


			std::cout << std::endl;
		}
		i++;

	}


}

void Mapdata::CreateMapObject(){

	for (int i = 0; i < layermax; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < width; k++) {
				if (layer[i].maptip[j][k] >0) {

					Object  *objtemp = new Object;
					objtemp->AddComponent<RectPolygon>("magic_square")->SetSize(Vector2(10, 10));
					objtemp->transform.position = Vector3(k*10 - 300, -j*10+500, i*10);
					objtemp->transform.scale = Vector3::one*1.f;
					mapobj.push_back(objtemp);
				}
			}
		}
	}
}

void Mapdata::Perse(std::ifstream ifs , std::string str)
{



}
