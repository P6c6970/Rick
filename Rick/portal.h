#pragma once
#include "SFML/Graphics.hpp"

class Portal {
public:
	short position[2][2];
	short x = 112;
	short y = 180;

	sf::Texture portalTexture;
	sf::Sprite portalSprite;

	void start() {
		position[0][0] = -32768;
		position[0][1] = -32768;
		position[1][0] = -32768;
		position[1][1] = -32768;
		portalTexture.loadFromFile("images/portalImage.png");
		portalSprite.setTexture(portalTexture);
	}

	void setPosition(float x, float y) {
		portalSprite.setPosition(x, y);
	}

	short check(sf::String *TileMap, float x, float y) {//проверяем новую координату на наличие чего-то например стены
		for (int i = x / 45; i < (x + this->x) / 45; i++)
		{
			for (int j = y / 45; j < (y + this->y) / 45; j++)
			{
				if (TileMap[j][i] == '-') return 1;
			}
		}
		return 0;
	}

	bool setPosition(sf::String *TileMap, float x, float y) {
		if (check(TileMap, x, y) == 0) {
			portalSprite.setPosition(x, y);
			return true;
		}
		return false;
	}

	bool setPortal(sf::String *TileMap, short &x, short &y) {
		x -= this->x / 2.0;
		y -= this->y / 2.0;
		if (x > 0 && y > 0) {
			if (position[0][0] == -32768) {
				if (setPosition(TileMap, x, y)) {
					position[0][0] = x;
					position[0][1] = y;
				}
				else return 1;
			}
			else if (position[1][0] == -32768) {
				if (position[0][0] != x && position[0][1] != y) {
					if (setPosition(TileMap, x, y)) {
						position[1][0] = x;
						position[1][1] = y;
					}
					else return 1;
				}
				else return 1;
			}
			else {
				position[0][0] = -32768;
				position[0][1] = -32768;
				position[1][0] = -32768;
				position[1][1] = -32768;
			}
		}
		else return 1;
		return 0;
	}
};