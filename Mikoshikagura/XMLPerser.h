#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>      // std::ostringstream
#include "Core\Core.h"

class maplayer {

public:
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

	std::vector<maplayer> layer;

	std::vector<Object*> mapobj;

public:
	Mapdata();								//コンストラクタ
	Mapdata(std::string str);
	~Mapdata() {};								//デストラクタ

	void Load(std::string);					//マップロード
	void SetMaptip(int x, int y, int value) {};//マップ変更

	bool IsCollison(int x, int y){};//マップ変更			//当たり判定

	void MapView();
	void CreateMapObject();
private:

	void Mapdata::Perse(std::ifstream ifs, std::string str);
	std::pair<int, int> WorldtoCell(Vector3 worldpos);			//ワールドとセルの変換
	std::pair<float, float> CelltoWorld(int valuex, int valuey) {};			//ワールドとセルの変換

	bool IsCollison(Vector3 pos);
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