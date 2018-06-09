#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>      // std::ostringstream
#include <map>
#include "Core\Core.h"
#include "player.h"

/*マップレイヤークラス*/
class maplayer {

public:

	std::string name;
	std::map<std::pair<int,int>, Object*> mapobj;	//マップオブジェクトのマップ
	std::vector<std::vector<int>> maptip;			//マップデータの二重配列

public:
	void Load() {};									//未実装
	int Gettip() {};								//未実装
	void Settip(int x,int y ,int value) {};			//未実装

};



/*マップ管理クラス*/
class MapManager :public Object {

	static constexpr int BlockSize = 10;			//ブロックサイズ

	int layermax;									//レイヤー数
	int width;										//幅
	int height;										//高さ
	Vector3 objscale;								//マップオブジェクトスケール
	Player *playerobj;								//プレイヤーのポインタ
	std::pair<int, int> playercell;					//プレイヤーのいるセル

	std::vector<maplayer> layer;					//マップデータ

public:
	MapManager();									//コンストラクタ
	MapManager(std::string str);
	~MapManager() {};								//デストラクタ

	virtual void Update();

	void Load(std::string);							//マップロード
	void SetMaptip(int x, int y, int value) {};		//マップ変更（未実装）

	void MapView();									//未実装
	void CreateMapObject();							//オブジェクト生成
	void UpdatePlayerCell();						//プレイヤーがいるセルの更新
	void SetActiveCollider(std::pair<int, int> cell, bool state);	//コライダーの更新
	void SetPlayerpointer(Player *player);			//プレイヤーポインタの設定

	void SetLayerActive(int layer, bool active);


private:

	void MapManager::Perse(std::ifstream ifs, std::string str);			
	std::pair<int, int> WorldtoCell(Vector3 worldpos);					//ワールドとセルの変換
	std::pair<float, float> CelltoWorld(int valuex, int valuey) {};		//セルとワールドの変換(未実装)


};

