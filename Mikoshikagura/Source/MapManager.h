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
enum class LayerType :int				//���C���[�^�C�v
{
	None = 0,
	Field,
	Accessory,
	Gimmick_Object
};
enum class GroupType :int				//�O���[�v�^�C�v
{
	None = 0,
	Static,
	Season,
	Summer,
	Winter,
};


/*�}�b�v���C���[�N���X*/
class MapLayer {
public:

	std::string name;
	LayerType layer;
	GroupType group;
	std::map<std::pair<int,int>, Object*> mapobj;	//�}�b�v�I�u�W�F�N�g�̃}�b�v
	std::vector<std::vector<int>> maptip;			//�}�b�v�f�[�^�̓�d�z��

	MapLayer();
	~MapLayer();

	void Load() {};									//������
	int Gettip() {};								//������
	void Settip(int x,int y ,int value) {};			//������

};



/*�}�b�v�Ǘ��N���X*/
class MapManager :public Object ,ISeason{


	static constexpr int BlockSize = 10;			//�u���b�N�T�C�Y

	int width;										//��
	int height;										//����
	Vector3 objscale;								//�}�b�v�I�u�W�F�N�g�X�P�[��
	Player *playerobj;								//�v���C���[�̃|�C���^
	std::pair<int, int> playercell;					//�v���C���[�̂���Z��

	//0612�i��@�}�b�v�̋G�ߕω�

	std::map<std::pair<int, int>, Object*> fieldobjectmap;
	std::vector<Object *> seasonobjectlist;			//�G�ߕω�
	std::vector<Object *> summerobjectlist;			//�ėp
	std::vector<Object *> winterobjectlist;			//�~�p
	std::vector<Object *> allobjectlist;
	std::vector<Object *> signobjectlist;
	std::vector<Object *> smoothobjectlist;
public:
	MapManager();									//�R���X�g���N�^
	virtual ~MapManager();									//�f�X�g���N�^

	virtual void Update();

	virtual void SetSummer();
	virtual void SetWinter();
	void Load(std::string);							//�}�b�v���[�h
	void SetMaptip(int x, int y, int value) {};		//�}�b�v�ύX�i�������j

	void CreateMap(MapLayer layer);
	void UpdatePlayerCell();						//�v���C���[������Z���̍X�V
	void SetActiveCollider(std::pair<int, int> cell, bool state);	//�R���C�_�[�̍X�V
	void SetPlayerpointer(Player *player);			//�v���C���[�|�C���^�̐ݒ�
	void SetSignText(Hukidashi * hukidasi);
	void SetSmoothPoint(MainCamera *camera);
	float GetGroundPosition(float x);
private:

	std::vector<float> groundheightlist;					//�n�\�̍������X�g
	std::vector<std::vector<int>> MapManager::Perse(std::string csvdata);
	LayerType SetLayerType(std::string layertype);
	GroupType SetGroupType(std::string grouptype);
	Object * CreateMapObject(int id,MapLayer layer);
	std::pair<int, int> WorldtoCell(Vector3 worldpos);					//���[���h�ƃZ���̕ϊ�

	Vector3 CelltoWorld(std::pair<int,int> cell);		//�Z���ƃ��[���h�̕ϊ�(������)


};

