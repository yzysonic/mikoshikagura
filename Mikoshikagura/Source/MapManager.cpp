#include "MapManager.h"
#include <tinyxml2.h>
#include "SeasonModel.h"

/////////////////MapLayer///////////////////


MapLayer::MapLayer()
{
}


MapLayer::~MapLayer()
{

	//マップオブジェクトの削除
	mapobj.clear();


	//マップチップの削除
	for (auto itr = maptip.begin(); itr != maptip.end(); ++itr) {
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

	seasonobjectlist.clear();
	seasonobjectlist.shrink_to_fit();
	winterobjectlist.clear();
	winterobjectlist.shrink_to_fit();
	summerobjectlist.clear();
	summerobjectlist.shrink_to_fit();

	fieldobjectmap.clear();


}



void MapManager::Load(std::string str)
{
	tinyxml2::XMLDocument xml;
	xml.LoadFile("Data/Map/prototype_map1.tmx");

	width = std::stoi(xml.FirstChildElement("map")->Attribute("width"));
	height = std::stoi(xml.FirstChildElement("map")->Attribute("height"));
	tinyxml2::XMLElement *xml_group = xml.FirstChildElement("map")->FirstChildElement("group");

	while (xml_group != nullptr) {
		MapLayer layerbuff;
		layerbuff.group = xml_group->Attribute("name");
		tinyxml2::XMLElement *xml_layer = xml_group->FirstChildElement("layer");

		while (xml_layer != nullptr) {
			CreateMapObject(xml_group->Attribute("name"), xml_layer->Attribute("name"), Perse(xml_layer->FirstChildElement("data")->GetText()));
			xml_layer = xml_layer->NextSiblingElement();
		}

		xml_group = xml_group->NextSiblingElement();

	}

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


	for (int i = 0; i < (int)vector.size(); i++) {

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


void MapManager::CreateMapObject(std::string groupname, std::string layername, std::vector<std::vector<int>> mapdata)
{

	enum class Object_type :int {
		Static,
		Season,
		Summer,
		Winter,
	};

	bool IsField = false;
	Object_type type;

	std::vector<Object*>::iterator itr;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			auto id = mapdata[i][j];
			if (id > 0) {

				Vector3 objscale;										//スケール
				Object  *objtemp = new Object;							//オブジェクト生成
				objtemp->type = ObjectType::Field;						//タイプ設定

				std::string model_name = "Maptip/" + std::to_string(id);	//名前設定




				objtemp->transform.scale = transform.scale;				//スケール設定
				objscale = objtemp->transform.scale;

				//位置計算
				//objtemp->transform.position = Vector3((float)(k * BlockSize * objscale.x), (float)((height - j) * BlockSize * objscale.y), float(i* objscale.z * BlockSize));
				objtemp->transform.position = Vector3((float)(j * BlockSize * objscale.x), (float)((height - i) * BlockSize * objscale.y), 0.0f);

				objtemp->AddComponent<BoxCollider2D>();					//コライダー追加
				objtemp->GetComponent<BoxCollider2D>()->size = Vector2(BlockSize * objscale.x, BlockSize * objscale.y);
				objtemp->GetComponent<BoxCollider2D>()->SetActive(false);

				if (groupname == "Season") {
					objtemp->AddComponent<SeasonModel>(model_name.c_str(),true);
				} else if (!ModelData::Get(model_name)) {
					model_name = "field_summer"; 
					objtemp->AddComponent<StaticModel>(model_name);
				} else {
					objtemp->AddComponent<StaticModel>(model_name);
				}


				//フィールドレイヤーの場合mapに格納
				if (layername == "Field") {
					fieldobjectmap[std::pair<int, int>(j, i)] = objtemp;
				}

				//グループごとにリストにポインタを格納
				if (groupname == "Season") {
					seasonobjectlist.push_back(objtemp);
				} else if (groupname == "Summer") {
					objtemp->SetActive(false);
					summerobjectlist.push_back(objtemp);

				} else if (groupname == "Winter") {
					objtemp->SetActive(false);
					winterobjectlist.push_back(objtemp);
				}
			}
		}
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


	for (auto itcell : targetcell) {
		//エラー回避
		if (itcell.first < 0 || itcell.first >= width ||
			itcell.second < 0 || itcell.second >= height) {
			continue;
		}

		if (fieldobjectmap.find(itcell) != fieldobjectmap.end()) {
			if (fieldobjectmap[itcell]->GetActive()) {
				fieldobjectmap[itcell]->GetComponent<BoxCollider2D>()->SetActive(state);
			}
		}

	}


}

void MapManager::SetPlayerpointer(Player *player)
{
	playerobj = player;
}

void MapManager::Update()
{
	if (playerobj != nullptr) {
		UpdatePlayerCell();

	}

}

void MapManager::SetSummer()
{
	for (auto itr :summerobjectlist) {
		itr->SetActive(true);
	}

	for (auto itr : winterobjectlist) {
		itr->SetActive(false);
	}
}

void MapManager::SetWinter()
{
	for (auto itr : winterobjectlist) {
		itr->SetActive(true);
	}

	for (auto itr : summerobjectlist) {
		itr->SetActive(false);
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




