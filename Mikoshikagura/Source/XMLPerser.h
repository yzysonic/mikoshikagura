#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>      // std::ostringstream
#include <map>
#include "Core\Core.h"
#include "player.h"
class maplayer {

public:

	std::map<std::pair<int,int>, Object*> mapobj;
	std::vector<std::vector<int>> maptip;

public:
	void Load() {};
	int Gettip() {};
	void Settip(int x,int y ,int value) {};

};



class Mapdata :public Object {

	int layermax;
	int width;
	int height;
	Vector3 objscale;
	Player *playerobj;
	std::pair<int, int> playercell;

	std::vector<maplayer> layer;

public:
	Mapdata();								//コンストラクタ
	Mapdata(std::string str);
	~Mapdata() {};							//デストラクタ

	virtual void Update();

	void Load(std::string);					//マップロード
	void SetMaptip(int x, int y, int value) {};//マップ変更

	Vector3 Mapdata::IsCollison(Vector3 position, Vector3 control, Vector2 size);

	void MapView();
	void CreateMapObject();
	void UpdatePlayerCell();
	void SetActiveCollider(std::pair<int, int> cell, bool state);
	void SetPlayerpointer(Player *player);


private:

	void Mapdata::Perse(std::ifstream ifs, std::string str);
	std::pair<int, int> WorldtoCell(Vector3 worldpos);			//ワールドとセルの変換
	std::pair<float, float> CelltoWorld(int valuex, int valuey) {};			//ワールドとセルの変換



	void SetLayerActive(int layer ,bool active);
};


//
//
//class  XMLelement
//{
//private:
//	XMLelement *parent;
//	std::vector<*(XMLelement)> child;
//
//	template <typename T>
//	std::pair < std::string, T >;
//	
//public:
//	XMLelement();
//	~ XMLelement();
//
//private:
//
//
//
//
//};
//
//
//
//class XMLPerser {
//
//
//private:
//	XMLelement *root;
//
//	//xmlの読み込み関数
//	void Load();
//	void Perse();
//
//
//public:
//	template <typename T>
//	T find(std::string tag);
//
//
//
//
//
//};