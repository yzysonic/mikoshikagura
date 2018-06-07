#include "XMLPerser.h"

Mapdata::Mapdata() {


	this->name = "mapdata";

}

Mapdata::Mapdata(std::string str)
{
	std::ifstream ifs(str);
	maplayer mapbaff;
	//transform.scale = Vector3(0.2f, 0.2f, 0.2f);


	bool flag = false;

	if (ifs.fail()) {
		std::cerr << "Failed to open file." << std::endl;
		return;
	}


	while (std::getline(ifs, str)) {
		if (str.find("data") != -1) {
			if (flag) {
				flag = false;

				layer.push_back(mapbaff);
				mapbaff.maptip.clear();
			} else {
				flag = true;
			}
		} else {
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

	playerobj = nullptr;

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



Vector3 Mapdata::IsCollison(Vector3 position, Vector3 control,Vector2 size)
{
	position.x += 10;


	std::pair<int, int> cell = WorldtoCell(position);



	if (cell.first < 0 ) {
		control.x = 1.0f;
		return control;
	} else if (cell.first > layer[0].maptip[0].size()) {
		control.x = -1.0f;
		return control;
	}
	if (cell.second < 0) {
		control.y = -1.0f;
		return control;
	} else if (cell.second > layer[0].maptip.size()) {
		control.y = 1.0f;
		return control;
	}
	if ((cell.first + 1) < layer[0].maptip[0].size() && cell.first >= 0) {
		if (layer[0].maptip[cell.second][cell.first + 1] > 0 && control.x > 0.0f) {
			control.x = 0.0f;
		}
	}
	if ((cell.first - 1) > 0 && cell.first < layer[0].maptip[0].size()) {
		if (layer[0].maptip[cell.second][cell.first - 1] > 0 && control.x < 0.0f) {
			control.x = 0.0f;
		}
	}

	if ((cell.second + 1) < layer[0].maptip.size() && cell.second >= 0) {
		if (layer[0].maptip[cell.second + 1][cell.first] > 0 && control.y < 0.0f) {
			control.y = 0.0f;
		}
	}
	if ((cell.second - 1) > 0 &&  cell.second <  layer[0].maptip.size()) {
		if (layer[0].maptip[cell.second - 1][cell.first] > 0 && control.y > 0.0f) {
			control.y = 0.0f;
		}
	}


	return control;
}

void Mapdata::MapView()
{
	int i = 0;
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

void Mapdata::CreateMapObject() {

	for (int i = 0; i < layermax; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < width; k++) {
				if (layer[i].maptip[j][k] > 0) {

					Vector3 objscale;
					Object  *objtemp = new Object;
					objtemp->AddComponent<StaticModel>("field_summer");
					objtemp->transform.scale = transform.scale;
					objscale = objtemp->transform.scale;

					objtemp->transform.position = Vector3((float)(k * BlockSize * objscale.x), (float)((height-j) * BlockSize * objscale.y), float(i* objscale.z * BlockSize));
					//objtemp->transform.position.y += 2500 * objscale.x;

					objtemp->AddComponent<BoxCollider2D>();
					objtemp->GetComponent<BoxCollider2D>()->size = Vector2(BlockSize * objscale.x, BlockSize * objscale.y);
					objtemp->GetComponent<BoxCollider2D>()->SetActive(false);

					layer[i].mapobj[std::pair<int,int>(k,j)]= objtemp;
				}
			}
		}
	}
}

void Mapdata::UpdatePlayerCell()
{

	Vector3  playerpos = playerobj->transform.position;

	std::pair<int, int> celltemp = WorldtoCell(playerpos);

	if (celltemp != playercell) {


		SetActiveCollider(playercell, false);
		playercell = celltemp;
		SetActiveCollider(playercell, true);
	}

}

void Mapdata::SetActiveCollider(std::pair<int ,int> cell,bool state)
{

	std::pair<int, int> targetcell[5];
	targetcell[0] = cell;
	targetcell[0].first += 1;
	targetcell[1] = cell;
	targetcell[1].first -= 1;
	targetcell[2] = cell;
	targetcell[2].second += 1;
	targetcell[3] = cell;
	targetcell[3].second += 1;
	targetcell[3].first	-= 1;
	targetcell[4] = cell;
	targetcell[4].second += 1;
	targetcell[4].first += 1;
	//エラー回避

	for (int i = 0; i < 5; i++) {
		if (targetcell[i].first < 0  || targetcell[i].first >= layer[0].maptip[0].size() ||
			targetcell[i].second < 0 || targetcell[i].second >= layer[0].maptip.size()) {
			continue;
		}

		if (layer[0].maptip[targetcell[i].second][targetcell[i].first] != 0)
		{
			layer[0].mapobj[targetcell[i]]->GetComponent<BoxCollider2D>()->SetActive(state);
		}

	}

	//オブジェクト検索



}

void Mapdata::SetPlayerpointer(Player *player)
{
	playerobj = player;
}

void Mapdata::Update()
{

	if (GetKeyboardPress(DIK_1)) {

		SetLayerActive(0, true);
	}

	if (GetKeyboardPress(DIK_2)) {

		SetLayerActive(0, false);
	}
	if (playerobj != nullptr) {
		UpdatePlayerCell();

	}

}






void Mapdata::Perse(std::ifstream ifs, std::string str)
{



}

std::pair<int, int> Mapdata::WorldtoCell(Vector3 worldpos)
{
	int x, y;
	x = (int)(worldpos.x / (transform.scale.x * BlockSize));
	y = (int)((500 - worldpos.y) / (transform.scale.y * BlockSize));

	return std::pair<int, int>(x, y);
}



void Mapdata::SetLayerActive(int layernum, bool active) {

	if (layernum >= layer.size()) {
		return;
	}

	for (auto itr = layer[layernum].mapobj.begin(); itr != layer[layernum].mapobj.end(); itr++) {

		itr->second->SetActive(active);
	}



}






