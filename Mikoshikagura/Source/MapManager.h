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
#include "sign.h"
#include "Maincamera.h"
enum class LayerType :int				//レイヤータイプ
{
	None = 0,
	Field,
	Accessory,
	Gimmick_Object
};
enum class GroupType :int				//グループタイプ
{
	None = 0,
	Static,
	Season,
	Summer,
	Winter,
};


/*マップレイヤークラス*/
class MapLayer {
public:

	std::string name;
	LayerType layer;
	GroupType group;
	std::map<std::pair<int,int>, Object*> mapobj;	//マップオブジェクトのマップ
	std::vector<std::vector<int>> maptip;			//マップデータの二重配列

	MapLayer();
	~MapLayer();

	void Load() {};									//未実装
	int Gettip() {};								//未実装
	void Settip(int x,int y ,int value) {};			//未実装

};



/*マップ管理クラス*/
class MapManager :public Object ,ISeason{


	static constexpr int BlockSize = 10;			//ブロックサイズ

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
	std::vector<Object *> signobjectlist;
	std::vector<Object *> smoothobjectlist;
public:
	MapManager();									//コンストラクタ
	virtual ~MapManager();									//デストラクタ

	virtual void Update();

	virtual void SetSummer();
	virtual void SetWinter();
	void Load(std::string);							//マップロード
	void SetMaptip(int x, int y, int value) {};		//マップ変更（未実装）

	void CreateMap(MapLayer layer);
	void UpdatePlayerCell();						//プレイヤーがいるセルの更新
	void SetActiveCollider(std::pair<int, int> cell, bool state);	//コライダーの更新
	void SetPlayerpointer(Player *player);			//プレイヤーポインタの設定
	void SetSignText(Hukidashi * hukidasi);
	void SetSmoothPoint(MainCamera *camera);
	float GetGroundPosition(float x);
private:

	std::vector<float> groundheightlist;					//地表の高さリスト
	std::vector<std::vector<int>> MapManager::Perse(std::string csvdata);
	LayerType SetLayerType(std::string layertype);
	GroupType SetGroupType(std::string grouptype);
	Object * CreateMapObject(int id,MapLayer layer);
	std::pair<int, int> WorldtoCell(Vector3 worldpos);					//ワールドとセルの変換

	Vector3 CelltoWorld(std::pair<int,int> cell);		//セルとワールドの変換(未実装)


};

