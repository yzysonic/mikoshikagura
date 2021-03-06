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
	xml.LoadFile(str.c_str());

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

	groundheightlist.resize(width, -1.0f);

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
	else {
		return LayerType::None;
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
	else
	{
		return GroupType::None;
	}

}
//マップチップオブジェクト作成
Object* MapManager::CreateMapObject(int id, MapLayer layer) {

	Object  *objtemp;							//オブジェクト生成

												/*モデル読み込み処理*/
	std::string model_name = "Maptip/" + std::to_string(id);	//名前設定
	switch (id)
	{
	case 26:
		//ステージ2　地面
		objtemp = new Object();
		model_name = "Maptip/" + std::to_string(23);

		if (ModelData::Get(model_name)) {
			objtemp->AddComponent<StaticModel>(model_name);
		}
		else {
			model_name = "field_summer";
			objtemp->AddComponent<StaticModel>(model_name);
		}

		break;
	case 45:

		//看板用処理
		objtemp = new Sign();
		objtemp->type = ObjectType::Accessary;						//タイプ設定
		signobjectlist.push_back(objtemp);
		if (Texture::Get(model_name)) {
			objtemp->AddComponent<RectPolygon>(model_name, Layer::MASK);
		}

		objtemp->GetComponent<RectPolygon>()->SetSize(Vector2::one * 10);
		objtemp->transform.position.y += 5.f;
		objtemp->transform.position.z += 5.f;
		break;

	case 48:
		objtemp = new Object();
		model_name = "tree_tekito";
		if (ModelData::Get(model_name)) {

			objtemp->AddComponent<StaticModel>(model_name, Layer::PLAYER)->alphaTestEnable = false; //暫定
		}
		else {
			model_name = "field_summer";
			objtemp->AddComponent<StaticModel>(model_name);
		}


		break;

	case 99:
		objtemp = new Object();
		smoothobjectlist.push_back(objtemp);

		break;


	case 100:
		objtemp = new Object();
		model_name = "torii";
		if (ModelData::Get(model_name)) {

			objtemp->AddComponent<StaticModel>(model_name, Layer::PLAYER)->alphaTestEnable = false; //暫定
			objtemp->transform.setRotation(0.f, PI / 2, 0.f);
		}
		else {
			model_name = "field_summer";
			objtemp->AddComponent<StaticModel>(model_name);
		}
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

			objtemp->AddComponent<SeasonModel>(model_name.c_str(), false);
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


	objtemp->transform.scale = objscale;				//スケール設定

	objtemp->AddComponent<BoxCollider2D>();					//コライダー追加
	objtemp->GetComponent<BoxCollider2D>()->size = Vector2(BlockSize * objscale.x, BlockSize * objscale.y);
	objtemp->GetComponent<BoxCollider2D>()->offset.y = 5.f;
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

				Object* objtemp = CreateMapObject(id, layer);

				objtemp->transform.position += Vector3((float)(j * BlockSize * objscale.x), (float)((height - i) * BlockSize * objscale.y), 0.0f);


				//フィールドレイヤーの場合mapに格納
				if (layer.layer == LayerType::Field) {
					objtemp->transform.scale.z = objscale.z * 3;
					fieldobjectmap[std::pair<int, int>(j, i)] = objtemp;
				}

				//グループごとにリストにポインタを格納
				if (layer.group == GroupType::Season) {
					seasonobjectlist.push_back(objtemp);
				}
				else if (layer.group == GroupType::Summer) {
					objtemp->SetActive(false);
					summerobjectlist.push_back(objtemp);

				}
				else if (layer.group == GroupType::Winter) {
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


	//if (ImGui::DragFloat("MapObjScale", &objscale.z, 0.1f, 0.1f, 10.f)) {

	//	for (auto itr : fieldobjectmap) {
	//		itr.second->transform.scale.z = objscale.z;
	//	}

	//}




}

void MapManager::SetSummer()
{
	//季節変更での

	for (auto itr : seasonobjectlist) {
		itr->GetComponent<SeasonModel>()->SetSummer();
	}

	for (auto itr : summerobjectlist) {
		itr->SetActive(true);
	}

	for (auto itr : winterobjectlist) {
		itr->SetActive(false);
	}

	/////プレイヤーを上に押し出す処理

	Vector3  playerpos = playerobj->transform.position;
	std::pair<int, int> celltemp = WorldtoCell(playerpos);

	while (fieldobjectmap.find(celltemp) != fieldobjectmap.end()) {
		if (fieldobjectmap[celltemp]->GetActive())
		{
			playerpos.y += 10;
			playerobj->SetPosition(playerpos);
			celltemp.second++;
		}
		else
		{
			break;
		}

	}


	//マップ表面の初期化
	for (auto & x : groundheightlist)
	{
		x = -1.0f;
	}
}

void MapManager::SetWinter()
{

	for (auto itr : seasonobjectlist) {
		itr->GetComponent<SeasonModel>()->SetWinter();
	}


	for (auto itr : winterobjectlist) {
		itr->SetActive(true);
	}

	for (auto itr : summerobjectlist) {
		itr->SetActive(false);
	}

	/////プレイヤーを上に押し出す処理

	Vector3  playerpos = playerobj->transform.position;
	std::pair<int, int> celltemp = WorldtoCell(playerpos);

	while (fieldobjectmap.find(celltemp) != fieldobjectmap.end()) {
		if (fieldobjectmap[celltemp]->GetActive())
		{
			playerpos.y += 10;
			playerobj->SetPosition(playerpos);
			celltemp.second--;
		}
		else
		{
			break;
		}

	}

	//マップ表面の初期化
	for (auto &x : groundheightlist)
	{
		x = -1.0f;
	}

}

std::pair<int, int> MapManager::WorldtoCell(Vector3 worldpos)
{
	int x, y;
	worldpos.x += BlockSize / 2;
	x = (int)(worldpos.x / (objscale.x * BlockSize));
	y = (int)((height *  BlockSize - worldpos.y) / (objscale.y * BlockSize));

	return std::pair<int, int>(x, y);
}

Vector3 MapManager::CelltoWorld(std::pair<int, int> cell)
{

	Vector3 worldpos;
	worldpos.x = (float)cell.first * BlockSize;
	worldpos.y = (float)(height - cell.second) * BlockSize;
	worldpos.z = 0;
	return worldpos;
}




//////////////////////////////////////////////////ここからテスト//////////////////////////
void MapManager::SetSignText(Hukidashi* hukidasi) {

	tinyxml2::XMLDocument xml;

	xml.LoadFile("Data/Text/Sign_txt.xml");

	tinyxml2::XMLElement *xml_id = xml.FirstChildElement("root")->FirstChildElement("sign");

	for (auto itr : signobjectlist) {
		itr->GetComponent<BoxCollider2D>()->SetActive(true);
		dynamic_cast<Sign*>(itr)->Sign::SetText(xml_id->FirstChildElement("data")->GetText(), hukidasi);

		xml_id = xml_id->NextSiblingElement();
	}

}

void MapManager::SetSmoothPoint(MainCamera *camera)
{
	for (auto itr : smoothobjectlist) {
		camera->AddSnapper(&(itr->transform));
	}

}

float MapManager::GetGroundPosition(float x) {

	std::pair <int, int> celltemp = WorldtoCell(Vector3(x, 0, 0));


	if (celltemp.first < 0) {
		return 0.0f;
	}
	else if (celltemp.first >= width) {
		return 0.0f;
	}


	if (groundheightlist[celltemp.first] != -1.0f) {
		return groundheightlist[celltemp.first];
	}

	auto itr = fieldobjectmap.find(celltemp);

	while (itr == fieldobjectmap.end())
	{
		celltemp.second--;
		itr = fieldobjectmap.find(celltemp);

		if (celltemp.second < 0) {
			groundheightlist[celltemp.first] = 0.0f;
			return 0.0f;
		}
	}

	while (itr != fieldobjectmap.end()) {
		if (itr->second->GetActive()) {
			celltemp.second--;
			itr = fieldobjectmap.find(celltemp);
		}
		else {
			break;
		}
	}
	groundheightlist[celltemp.first] = CelltoWorld(celltemp).y - 5.0f;
	return groundheightlist[celltemp.first];
}