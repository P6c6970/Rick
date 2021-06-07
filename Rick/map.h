#pragma once
#include "SFML/Graphics.hpp"

#include <fstream>
#include <iostream>
class Map {
public:
	short TileMapX = 33;
	short TileMapY = 33;

	sf::String *TileMapFon;
	sf::String *TileMap;

	short positionHeroX;
	short positionHeroY;
	void load(bool &status, short a) {
		std::string line;
		std::ifstream fin;
		std::string lvl = "maps/lvl ";
		lvl += std::to_string(a);
		lvl += ".txt";
		fin.open(lvl);
		if (!fin.is_open()) {// ���� ���� �� ������
			status = false;
		}
		else {
			getline(fin, line);// ������� ������ �� �����
			TileMapX = atoi(line.c_str());
			getline(fin, line);// ������� ������ �� �����
			TileMapY = atoi(line.c_str());

			//��������� ������
			TileMapFon = new sf::String[TileMapY];
			TileMap = new sf::String[TileMapY];

			//��������� ��������������
			getline(fin, line);// ������� ������ �� �����
			positionHeroX = atoi(line.c_str());
			getline(fin, line);// ������� ������ �� �����
			positionHeroY = atoi(line.c_str());
			//
			for (short i = 0; i < TileMapY; i++)
			{
				getline(fin, line);
				TileMapFon[i] = line;
			}
			for (short i = 0; i < TileMapY; i++)
			{
				getline(fin, line);
				TileMap[i] = line;
			}
		}
		fin.close(); // ��������� ����
	}
};


//2 390              47
//581 406         626
sf::Texture fonTexture;
sf::Sprite fonSprite(fonTexture);
void startFon(sf::RenderWindow &window) {
	fonTexture.loadFromFile("images/fonImage.png");//1366 768
}
