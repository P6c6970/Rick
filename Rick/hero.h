#pragma once
#include "def.h"
#include "portal.h"
#include "map.h"
class Player {
public:
	short x = 124;
	short y = 161;
	short startX;
	short startY;
	bool lvl = false;
	float CurrentFrame = 0;//хранит текущий кадр
	sf::Texture heroTexture;
	sf::Sprite heroSprite;
	Portal portal;
	
	void start(short px, short py, std::string line) {
		createTexture(heroTexture, heroSprite, ("images/hero/spriteHero" + line + ".png").c_str(), 515, 496);
		heroSprite.setTextureRect(sf::IntRect(0, 0, x, y));//получили нужный нам прямоугольник
		startX = px;
		startY = py;
		setPosition(startX, startY);
		portal.start();
	}

	void updateFrame(short temp1, short temp2) {
		switch (temp1)
		{
		case 0://down
			heroSprite.setTextureRect(sf::IntRect(((x + 5) * temp2), 0, x, y));
			break;
		case 1://left
			heroSprite.setTextureRect(sf::IntRect(((x + 5) * temp2), 166, x, y));
			break;
		case 2://right
			heroSprite.setTextureRect(sf::IntRect(((x + 5) * temp2) + 124, 166, -x, y));
			break;
		case 3://up
			heroSprite.setTextureRect(sf::IntRect(((x + 5) * temp2), 332, x, y));
			break;
		}
	}

	void updateCurrentFrame(float temp) {
		CurrentFrame += temp; //служит для прохождения по "кадрам". переменная доходит до 4 суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
		if (CurrentFrame > 4) CurrentFrame -= 4;
	}

	void setPosition(float x, float y) {
		heroSprite.setPosition(x, y);
	}

	short check(Map &map, float x, float y) {//checking the new coordinate
		for (int i = (heroSprite.getPosition().x + x) / 45; i < (heroSprite.getPosition().x + x + this->x) / 45; i++)
		{
			for (int j = (heroSprite.getPosition().y + y) / 45; j < (heroSprite.getPosition().y + y + this->y) / 45; j++)
			{
				if (i >= 0 && j >= 0 && i < map.TileMapX && j < map.TileMapY) {
					if (map.TileMap[j][i] == '-') return 1;
					//если камень
					else if (map.TileMap[j][i] == 'k') {
						lvl = true;
						map.TileMap[j][i] = '0';
					}
					//если лазер или мина
					else if (map.TileMap[j][i] == 'q' || map.TileMap[j][i] == 'w' || map.TileMap[j][i] == 'e' || map.TileMap[j][i] == 'a' || map.TileMap[j][i] == 's' || map.TileMap[j][i] == 'd' || map.TileMap[j][i] == 'm') {
						portal.position[0][0] = -32768;
						portal.position[0][1] = -32768;
						portal.position[1][0] = -32768;
						portal.position[1][1] = -32768;
						setPosition(startX, startY);
						return 2;
					}
				}
				else return 1;
			}
		}
		return 0;
	}

	bool addPosition(Map &map, float x, float y) {
		int temp = check(map, x, y);
		if (temp == 0) {
			heroSprite.move(x, y);
		}
		else if (temp == 2) return 1;
		return 0;
	}

	//a.y < b.y1 || a.y1 > b.y || a.x1 < b.x || a.x > b.x1
	void checkPortal(Map &map, short direction) {
		if (portal.position[0][0] != -32768 && portal.position[1][0] != -32768) {//if both portals are standing
			for (int i = 0; i < 2; i++)
			{
				//if we tought the portals
				if ((portal.position[i][0] < heroSprite.getPosition().x + x) && (portal.position[i][0] + portal.x > heroSprite.getPosition().x) && (portal.position[i][1] < heroSprite.getPosition().y + y) && (portal.position[i][1] + portal.y > heroSprite.getPosition().y)) {
					switch (direction)
					{
					case 0://up
						if (check(map, portal.position[(i + 1) % 2][0] + portal.x / 2.0 - x / 2.0 - heroSprite.getPosition().x, portal.position[(i + 1) % 2][1] - y - 1 - heroSprite.getPosition().y) == 0) {
							if (portal.position[0][0] != -32768 && portal.position[1][0] != -32768)
								heroSprite.setPosition(portal.position[(i + 1) % 2][0] + portal.x / 2.0 - x / 2.0, portal.position[(i + 1) % 2][1] - y - 1);
						}
						break;
					case 1://down
						if (check(map, portal.position[(i + 1) % 2][0] + portal.x / 2.0 - x / 2.0 - heroSprite.getPosition().x, portal.position[(i + 1) % 2][1] + portal.y + 1 - heroSprite.getPosition().y) == 0) {
							if (portal.position[0][0] != -32768 && portal.position[1][0] != -32768)
								heroSprite.setPosition(portal.position[(i + 1) % 2][0] + portal.x / 2.0 - x / 2.0, portal.position[(i + 1) % 2][1] + portal.y + 1);
						}
						break;
					case 2://left
						if (check(map, portal.position[(i + 1) % 2][0] - 1 - x - heroSprite.getPosition().x, portal.position[(i + 1) % 2][1] + portal.y / 2.0 - y / 2.0 - heroSprite.getPosition().y) == 0) {
							if (portal.position[0][0] != -32768 && portal.position[1][0] != -32768)
								heroSprite.setPosition(portal.position[(i + 1) % 2][0] - 1 - x, portal.position[(i + 1) % 2][1] + portal.y / 2.0 - y / 2.0);
						}
						break;
					case 3://right
						if (check(map, portal.position[(i + 1) % 2][0] + portal.x + 1 - heroSprite.getPosition().x, portal.position[(i + 1) % 2][1] + portal.y / 2.0 - y / 2.0 - heroSprite.getPosition().y) == 0) {
							if (portal.position[0][0] != -32768 && portal.position[1][0] != -32768)
								heroSprite.setPosition(portal.position[(i + 1) % 2][0] + portal.x + 1, portal.position[(i + 1) % 2][1] + portal.y / 2.0 - y / 2.0);
						}
						break;
					}
					break;
				}
			}

		}

	}
};