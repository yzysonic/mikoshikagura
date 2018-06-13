#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>      // std::ostringstream
#include <map>
#include "Core\Core.h"
#include "player.h"
#include "ISeason.h"
/*マップレイヤークラス*/
class MapLayer {

public:

	std::string name;
	std::string group;
	std::map<std::pair<int,int>, Object*> mapobj;	//マップオブジェクトのマップ
	std::vector<std::vector<int>> maptip;			//マップデータの二重配列

public:
	MapLayer();
	~MapLayer();

	void Load() {};									//未実装
	int Gettip() {};								//未実装
	void Settip(int x,int y ,int value) {};			//未実装

};



/*マップ管理クラス*/
class MapManager :public Object ,ISeason{

	static constexpr int BlockSize = 10;			//ブロックサイズ

	int layermax;									//レイヤー数
	int width;										//幅
	int height;										//高さ
	Vector3 objscale;								//マップオブジェクトスケール
	Player *playerobj;								//プレイヤーのポインタ
	std::pair<int, int> playercell;					//プレイヤーのいるセル

	//0612永井　マップの季節変化

	std::map<std::pair<int, int>, Object*> fieldobjectmap;
	std::vector<Object *> seasonobjectlist;			//季節変化
	std::vector<Object *> summerobjectlist;			//夏用
	std::vector<Object *> winterobjectlist;			//冬用
	std::vector<Object *> allobjectlist;


public:
	MapManager();									//コンストラクタ
	virtual ~MapManager();									//デストラクタ

	virtual void Update();

	virtual void SetSummer();
	virtual void SetWinter();
	void Load(std::string);							//マップロード
	void SetMaptip(int x, int y, int value) {};		//マップ変更（未実装）

	void CreateMapObject(std::string group, std::string layer, std::vector<std::vector<int>> mapdata);
	void UpdatePlayerCell();						//プレイヤーがいるセルの更新
	void SetActiveCollider(std::pair<int, int> cell, bool state);	//コライダーの更新
	void SetPlayerpointer(Player *player);			//プレイヤーポインタの設定


private:

	std::vector<std::vector<int>> MapManager::Perse(std::string csvdata);
	std::pair<int, int> WorldtoCell(Vector3 worldpos);					//ワールドとセルの変換
	std::pair<float, float> CelltoWorld(int valuex, int valuey) {};		//セルとワールドの変換(未実装)


};

