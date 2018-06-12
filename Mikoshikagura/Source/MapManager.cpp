#include "MapManager.h"
#include <tinyxml2.h>


/////////////////MapLayer///////////////////


MapLayer::MapLayer()
{
}


MapLayer::~MapLayer()
{

	//マップオブジェクトの削除
	mapobj.clear();


	//マップチップの削除
	for (auto itr = maptip.begin(); itr != maptip.end(); ++itr)
	{
		itr->clear();
		itr->shrink_to_fit();
	}

	maptip.clear();
	maptip.shrink_to_fit();
}









//////////////////MapManager///////////////////

MapManager::MapManager() {

	this->name = "mapmanager";
}

MapManager::~MapManager()
{

	layer.clear();
	layer.shrink_to_fit();

}



void MapManager::Load(std::string str)
{
	tinyxml2::XMLDocument xml;
	xml.LoadFile("Data/Map/prototype_map1.tmx");
	MapLayer layerbuff;
	tinyxml2::XMLElement * elem = xml.FirstChildElement("map")->FirstChildElement("layer");
	while (elem !=nullptr) {

		layerbuff.maptip = Perse(elem->FirstChildElement("data")->GetText());	//csvパース
		layerbuff.name = elem->Attribute("name");

		layer.push_back(layerbuff);

		elem = elem->NextSiblingElement();

	}

	layermax = layer.size();
	height = layer[0].maptip.size();
	width = layer[0].maptip[0].size();
	CreateMapObject();

}
//csvデータのパース
std::vector<std::vector<int>> MapManager::Perse(std::string csvdata) {


	std::vector<std::string> vector;

	std::stringstream ss(csvdata);

	std::string csvline;
	std::vector<int> xbuff;
	std::vector<std::vector<int>> mapbuff;
	MapLayer layerbuff;
	while (std::getline(ss, csvline, '\n')) {
		if (csvline != "") {
			vector.push_back(csvline);
		}
	}


	for (int i = 0; i < vector.size(); i++) {

		std::stringstream ss2(vector[i]);
		std::string x;

		std::cout << ss.str();

		while (std::getline(ss2, x, ',')) {
			xbuff.push_back(std::stoi(x));
		}

		mapbuff.push_back(xbuff);
		xbuff.clear();

	}

	return mapbuff;

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
				auto id = layer[i].maptip[j][k];
				if (id > 0) {

					Vector3 objscale;
					Object  *objtemp = new Object;
					objtemp->type = ObjectType::Field;

					auto model_name = "field_block_" + std::to_string(id);
					if (!ModelData::Get(model_name))
						model_name = "field_summer";
					objtemp->AddComponent<StaticModel>(model_name);

					objtemp->transform.scale = transform.scale;
					objscale = objtemp->transform.scale;

					objtemp->transform.position = Vector3((float)(k * BlockSize * objscale.x), (float)((height - j) * BlockSize * objscale.y), float(i* objscale.z * BlockSize));

					objtemp->AddComponent<BoxCollider2D>();
					objtemp->GetComponent<BoxCollider2D>()->size = Vector2(BlockSize * objscale.x, BlockSize * objscale.y);

					layer[i].mapobj[std::pair<int, int>(k, j)] = objtemp;
				}
			}
		}
	}

	for (int i = 0; i < layer.size(); i++) {
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

void MapManager::SetActiveCollider(std::pair<int, int> cell, bool state)
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
	targetcell[3].first -= 1;
	targetcell[4] = cell;
	targetcell[4].second += 1;
	targetcell[4].first += 1;


	for (int i = 0; i < 5; i++) {
		//エラー回避
		if (targetcell[i].first < 0 || targetcell[i].first >= layer[0].maptip[0].size() ||
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

