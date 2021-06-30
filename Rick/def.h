#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#define pow2(x) ((x)*(x))

enum Color {
	POISONOUS_GREEN,
	ORANGE,
	SKY_BLUE,
	RUBY
};

void setColor(sf::Color &color, int &r, int &g, int &b) {//Задать цвет
	color.r = r;
	color.g = g;
	color.b = b;
}

void setColor(sf::Color &color, Color temp) {//Задать цвет
	switch (temp)
	{
	case POISONOUS_GREEN:
		color.r = 58;
		color.g = 246;
		color.b = 46;
		break;
	case ORANGE:
		color.r = 255;
		color.g = 69;
		color.b = 0;
		break;
	case SKY_BLUE:
		color.r = 0;
		color.g = 191;
		color.b = 255;
		break;
	case RUBY:
		color.r = 202;
		color.g = 1;
		color.b = 71;
		break;
	default:
		color.r = 255;
		color.g = 255;
		color.b = 255;
		break;
	}
}

void setView(sf::RenderWindow &window, sf::View &view, float x, float y, short &TileMapX, short &TileMapY) {
	if (x < window.getSize().x / 2) {
		x = window.getSize().x / 2;
	}
	else if (x > TileMapX * 45 - window.getSize().x / 2) {
		x = TileMapX * 45 - window.getSize().x / 2;
	}
	if (y < window.getSize().y / 2) {
		y = window.getSize().y / 2;
	}
	else if (y > TileMapY * 45 - window.getSize().y / 2) {
		y = TileMapY * 45 - window.getSize().y / 2;
	}
	view.setCenter(x, y);
}

void soundGameOverS(std::string &soundStatus, sf::Sound &sound) {
	if (soundStatus == "1") {
		sound.play();
	}
}

void createTexture(sf::Texture &texture, sf::Sprite &sprite, const char* adress, short x, short y) {
	if (!texture.loadFromFile(adress)) {//если файла нет рисуем квадрат
		texture.create(x, y);
	}
	sprite.setTexture(texture);
}