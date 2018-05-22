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

void Mapdata::Perse(std::ifstream ifs , std::string str)
{



}
