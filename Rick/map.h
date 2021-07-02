#pragma once
#include "def.h"
class Map {
public:
	short TileMapX = 33;
	short TileMapY = 33;

	sf::String *TileMapFon;
	sf::String *TileMap;

	sf::Texture fonTexture;
	sf::Sprite fonSprite;

	short positionHeroX;
	short positionHeroY;
	void load(bool &status, short a) {
		createTexture(fonTexture, fonSprite, "images/fonImage.png", 966, 520);
		std::string line;
		std::ifstream fin;
		std::string lvl = "maps/lvl ";
		lvl += std::to_string(a);
		lvl += ".txt";
		fin.open(lvl);
		if (!fin.is_open()) {//if the file isn't open
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