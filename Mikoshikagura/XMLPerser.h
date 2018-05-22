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


class Mapdata :public Script {


	int layermax;
	int width;
	int height;

	std::vector<maplayer> layer;

public:
	Mapdata();								//�R���X�g���N�^
	Mapdata(std::string str);
	~Mapdata() {};								//�f�X�g���N�^


	void Load(std::string);					//�}�b�v���[�h
	void SetMaptip(int x, int y, int value) {};//�}�b�v�ύX


	bool IsCollison(int x, int y){};//�}�b�v�ύX			//�����蔻��

	void MapView();
private:

	void Mapdata::Perse(std::ifstream ifs, std::string str);
	std::pair<int, int> WorldtoCell(float valuex, float valuey) {};			//���[���h�ƃZ���̕ϊ�
	std::pair<float, float> CelltoWorld(int valuex, int valuey) {};			//���[���h�ƃZ���̕ϊ�
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
//	//xml�̓ǂݍ��݊֐�
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