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
			getline(fin, line);// считали строку из файла
			TileMapX = atoi(line.c_str());
			getline(fin, line);// считали строку из файла
			TileMapY = atoi(line.c_str());

			//обьявляем массив
			TileMapFon = new sf::String[TileMapY];
			TileMap = new sf::String[TileMapY];

			//стартовое местоположение
			getline(fin, line);// считали строку из файла
			positionHeroX = atoi(line.c_str());
			getline(fin, line);// считали строку из файла
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
		fin.close(); // закрываем файл
	}
};