#include "MapManager.h"

MapManager::MapManager() {

	this->name = "mapmanager";
}

MapManager::MapManager(std::string str)
{
	std::ifstream ifs(str);
	maplayer mapbuff;

	std::string layername;
	bool flag = false;

	if (ifs.fail()) {
		std::cerr << "Failed to open file." << std::endl;
		return;
	}


	//“Ç‚Ýž‚Ý•”
	while (std::getline(ifs, str)) {


		if(str.find("<layer") != -1){
			std::stringstream ss(str);

			std::getline(ss, layername, '"');
			std::getline(ss, layername, '"');
			mapbuff.name = layername;
		}				
		if (str.find("data") != -1) {
			if (flag) {
				flag = false;

				layer.push_back(mapbuff);
				mapbuff.maptip.clear();
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

				mapbuff.maptip.push_back(xbuff);

			}


		}

	}



	layermax = layer.size();
	height = layer[0].maptip.size();
	width = layer[0].maptip[0].size();

	playerobj = nullptr;

}

void MapManager::Load(std::string str)
{
	std::ifstream ifs(str);

	if (ifs.fail()) {
		std::cerr << "Failed to open file." << std::endl;

	}


	while (std::getline(ifs, str)) {
		std::cout << "#" << str << std::endl;


	}

}


void MapManager::MapView()
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

void MapManager::CreateMapObject() {

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

					objtemp->AddComponent<BoxCollider2D>();
					objtemp->GetComponent<BoxCollider2D>()->size = Vector2(BlockSize * objscale.x, BlockSize * objscale.y);
					objtemp->GetComponent<BoxCollider2D>()->SetActive(false);

					layer[i].mapobj[std::pair<int,int>(k,j)]= objtemp;
				}
			}
		}
	}

	for (int i = 0; i < layer.size(); i++)
	{
		SetLayerActive(i, false);
	}
}

void MapManager::UpdatePlayerCell()
{

	Vector3  playerpos = playerobj->transform.position;

	std::pair<int, int> celltemp = WorldtoCell(playerpos);

	if (celltemp != playercell) {


		SetActiveCollider(playercell, false);
		playercell = celltemp;
		SetActiveCollider(playercell, true);
	}

}

void MapManager::SetActiveCollider(std::pair<int ,int> cell,bool state)
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


	for (int i = 0; i < 5; i++) {
		//ƒGƒ‰[‰ñ”ð
		if (targetcell[i].first < 0  || targetcell[i].first >= layer[0].maptip[0].size() ||
			targetcell[i].second < 0 || targetcell[i].second >= layer[0].maptip.size()) {
			continue;
		}

		if (layer[0].maptip[targetcell[i].second][targetcell[i].first] != 0)
		{
			layer[0].mapobj[targetcell[i]]->GetComponent<BoxCollider2D>()->SetActive(state);
		}

	}


}

void MapManager::SetPlayerpointer(Player *player)
{
	playerobj = player;
}

void MapManager::Update()
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






void MapManager::Perse(std::ifstream ifs, std::string str)
{



}

std::pair<int, int> MapManager::WorldtoCell(Vector3 worldpos)
{
	int x, y;
	worldpos.x += BlockSize / 2;
	x = (int)(worldpos.x / (transform.scale.x * BlockSize));
	y = (int)((height *  BlockSize - worldpos.y) / (transform.scale.y * BlockSize));

	return std::pair<int, int>(x, y);
}



void MapManager::SetLayerActive(int layernum, bool active) {

	if (layernum >= layer.size()) {
		return;
	}

	for (auto itr = layer[layernum].mapobj.begin(); itr != layer[layernum].mapobj.end(); itr++) {

		itr->second->SetActive(active);
	}



}






