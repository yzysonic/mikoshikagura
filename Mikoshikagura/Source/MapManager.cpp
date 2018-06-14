#include "MapManager.h"
#include <tinyxml2.h>
#include "SeasonModel.h"
#include "Sign.h"

#pragma comment(lib, "tinyxml2.lib")

/////////////////MapLayer///////////////////


MapLayer::MapLayer()
{
}


MapLayer::~MapLayer()
{

}







//////////////////MapManager///////////////////

MapManager::MapManager() {

	this->name = "mapmanager";
}

MapManager::~MapManager()
{
	//for (auto itr : allobjectlist) {
	//	itr->Destroy();
	//}
	allobjectlist.shrink_to_fit();
	for (auto itr : seasonobjectlist) {

	}
	seasonobjectlist.shrink_to_fit();
	for (auto itr : summerobjectlist) {

	}
	summerobjectlist.shrink_to_fit();
	for (auto itr : winterobjectlist) {

	}
	winterobjectlist.shrink_to_fit();
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
		tinyxml2::XMLElement *xml_layer = xml_group->FirstChildElement("layer");

		while (xml_layer != nullptr) {
			MapLayer layerbuff;
			layerbuff.group = SetGroupType(xml_group->Attribute("name"));
			layerbuff.layer = SetLayerType(xml_layer->Attribute("name"));
			layerbuff.maptip = Perse(xml_layer->FirstChildElement("data")->GetText());

			CreateMap(layerbuff);
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

LayerType MapManager::SetLayerType(std::string layertype) {

	if (layertype == "Field") {
		return LayerType::Field;
	}
	else if (layertype == "Accessory") {

		return LayerType::Accessory;
	}
	else if (layertype == "Gimmick_Object") {
		return LayerType::Gimmick_Object;
	}
}


GroupType MapManager::SetGroupType(std::string grouptype) {
	if (grouptype == "Season") {
		return GroupType::Season;
	}
	else if (grouptype == "Summer") {

		return GroupType::Summer;
	}
	else if (grouptype == "Winter") {
		return GroupType::Winter;
	}
	else if (grouptype == "Static") {
		return GroupType::Static;
	}

}
//マップチップオブジェクト作成
Object* MapManager::CreateMapObject(int id , MapLayer layer) {

	Vector3 objscale;										//スケール
	Object  *objtemp;							//オブジェクト生成

												/*モデル読み込み処理*/
	std::string model_name = "Maptip/" + std::to_string(id);	//名前設定
	switch (id)
	{

	case 45:

		//看板用処理
		objtemp = new Sign();
		objtemp->type = ObjectType::Accessary;						//タイプ設定
		signobjectlist.push_back(objtemp);
		if (Texture::Get(model_name)) {
			objtemp->AddComponent<RectPolygon>(model_name,Layer::MASK);
		}
		else {
			
		}
		
		objtemp->GetComponent<RectPolygon>()->SetSize(Vector2::one *10);

		break;
	default:
		objtemp = new Object();
		objtemp->type = ObjectType::Field;						//タイプ設定

														//モデルエラーチェック
		switch (layer.group)
		{
		case GroupType::Season:
			if (!(ModelData::Get(model_name + "_summer")) && !(ModelData::Get(model_name + "_winter"))) {
				model_name = "field_summer";
			}

			objtemp->AddComponent<SeasonModel>(model_name.c_str());
			break;
		default:
			if (!(ModelData::Get(model_name))) {
				model_name = "field_summer";
			}

			objtemp->AddComponent<StaticModel>(model_name);
			break;
		}
		break;
	}


	objtemp->transform.scale = transform.scale;				//スケール設定
	objscale = objtemp->transform.scale;

	objtemp->AddComponent<BoxCollider2D>();					//コライダー追加
	objtemp->GetComponent<BoxCollider2D>()->size = Vector2(BlockSize * objscale.x, BlockSize * objscale.y);
	objtemp->GetComponent<BoxCollider2D>()->SetActive(false);


	return objtemp;
}

void MapManager::CreateMap(MapLayer layer)
{

	objscale = Vector3(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int id = layer.maptip[i][j];
			if (id > 0) {

				Object* objtemp = CreateMapObject(id,layer);

				objtemp->transform.position = Vector3((float)(j * BlockSize * objscale.x), (float)((height - i) * BlockSize * objscale.y), 0.0f);


				//フィールドレイヤーの場合mapに格納
				if (layer.layer == LayerType::Field) {
					fieldobjectmap[std::pair<int, int>(j, i)] = objtemp;
				}

				//グループごとにリストにポインタを格納
				if (layer.group == GroupType::Season) {
					seasonobjectlist.push_back(objtemp);
				} else if (layer.group == GroupType::Summer) {
					objtemp->SetActive(false);
					summerobjectlist.push_back(objtemp);

				} else if (layer.group == GroupType::Winter) {
					objtemp->SetActive(false);
					winterobjectlist.push_back(objtemp);
				}
				allobjectlist.push_back(objtemp);
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




//////////////////////////////////////////////////ここからテスト//////////////////////////
void MapManager::SetSignText(Hukidashi* hukidasi) {

	tinyxml2::XMLDocument xml;

	xml.LoadFile("Data/Text/Sign_txt.xml");

	tinyxml2::XMLElement *xml_id = xml.FirstChildElement("root")->FirstChildElement("sign");

	for (auto itr : signobjectlist) {
		itr->GetComponent<BoxCollider2D>()->SetActive(true);
		dynamic_cast<Sign*>(itr)->Sign::SetText(xml_id->FirstChildElement("data")->GetText(),hukidasi);

		xml_id = xml_id->NextSiblingElement();
	}

}