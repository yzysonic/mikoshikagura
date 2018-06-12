#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>      // std::ostringstream
#include <map>
#include "Core\Core.h"
#include "player.h"

/*�}�b�v���C���[�N���X*/
class MapLayer {

public:

	std::string name;
	std::map<std::pair<int,int>, Object*> mapobj;	//�}�b�v�I�u�W�F�N�g�̃}�b�v
	std::vector<std::vector<int>> maptip;			//�}�b�v�f�[�^�̓�d�z��

public:
	MapLayer();
	~MapLayer();

	void Load() {};									//������
	int Gettip() {};								//������
	void Settip(int x,int y ,int value) {};			//������

};



/*�}�b�v�Ǘ��N���X*/
class MapManager :public Object {

	static constexpr int BlockSize = 10;			//�u���b�N�T�C�Y

	int layermax;									//���C���[��
	int width;										//��
	int height;										//����
	Vector3 objscale;								//�}�b�v�I�u�W�F�N�g�X�P�[��
	Player *playerobj;								//�v���C���[�̃|�C���^
	std::pair<int, int> playercell;					//�v���C���[�̂���Z��

	std::vector<MapLayer> layer;					//�}�b�v�f�[�^


	//0612�i��@�}�b�v�̋G�ߕω�

	std::map<std::pair<int, int>, Object*> fieldobject;
	std::vector<Object *> seasonobjectlist;



public:
	MapManager();									//�R���X�g���N�^
	virtual ~MapManager();									//�f�X�g���N�^

	virtual void Update();

	void Load(std::string);							//�}�b�v���[�h
	void SetMaptip(int x, int y, int value) {};		//�}�b�v�ύX�i�������j

	void MapView();									//������
	void CreateMapObject();							//�I�u�W�F�N�g����
	void UpdatePlayerCell();						//�v���C���[������Z���̍X�V
	void SetActiveCollider(std::pair<int, int> cell, bool state);	//�R���C�_�[�̍X�V
	void SetPlayerpointer(Player *player);			//�v���C���[�|�C���^�̐ݒ�

	void SetLayerActive(int layer, bool active);


private:

	std::vector<std::vector<int>> MapManager::Perse(std::string csvdata);
	std::pair<int, int> WorldtoCell(Vector3 worldpos);					//���[���h�ƃZ���̕ϊ�
	std::pair<float, float> CelltoWorld(int valuex, int valuey) {};		//�Z���ƃ��[���h�̕ϊ�(������)


};
