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

					Vector3 objscale;
					Object  *objtemp = new Object;
					objtemp->AddComponent<StaticModel>("field");
					objtemp->transform.scale = Vector3(0.2f, 0.2f, 0.2f);
					objscale = objtemp->transform.scale;

					objtemp->transform.position = Vector3((float)(k * 100 * objscale.x), (float)(- j * 100 * objscale.y), float(i* objscale.z * 100));
					objtemp->transform.position.y += 5000 * objscale.x;
					objtemp->transform.position.x -= 5000 * objscale.y;
					mapobj.push_back(objtemp);
				}
			}
		}
	}
}

void Mapdata::Perse(std::ifstream ifs , std::string str)
{



}

std::pair<int, int> Mapdata::WorldtoCell(Vector3 worldpos)
{
	int x, y;
	x = (int)(worldpos.x / transform.scale.x);
	y = (int)(worldpos.y / transform.scale.y);

	return std::pair<int, int>(x,y);
}

bool Mapdata::IsCollison(Vector3 pos)
{

	std::pair<int, int> cellpos = WorldtoCell(pos);

	if (layer[0].maptip[cellpos.second + 1][cellpos.first] > 0) {
		return true;
	}

	return false;
}

