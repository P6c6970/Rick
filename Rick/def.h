#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "map.h"//�����
#include "hero.h"//����� � ������

enum Color {
	POISONOUS_GREEN,
	ORANGE,
	SKY_BLUE
};

double pow2(double temp) {
	return temp * temp;
}

void setColor(sf::Color &color, int &r, int &g, int &b) {//������ ����
	color.r = r;
	color.g = g;
	color.b = b;
}

void setColor(sf::Color &color, Color temp) {//������ ����
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

void printGame(sf::RenderWindow &window, short &position) {//��������� �������
	sf::Color color;
	//����� � �����
	sf::Font font;//����� 
	font.loadFromFile("font.ttf");//�������� ������ ������ ���� ������

	sf::Text textM;
	textM = sf::Text("Back", font, 50);
	//textM.setStyle(sf::Text::Bold);
	setColor(color, POISONOUS_GREEN);
	textM.setFillColor(color);//��������� �����. ���� ������ ��� ������, �� �� ��������� �� �����
	
	bool status = true;

	short selectedText = -1;
	short pressedText = -1;

	Map map;
	map.load(status, position - 3);
	if (status == false) position = 3;
	startFon(window);//��������� ������ ����

	

	sf::Texture stoneTexture;
	if (!stoneTexture.loadFromFile("images/stoneImage.png")) {//���� ����� ��� ������ �������
		stoneTexture.create(45, 45);
	}
	sf::Sprite stoneSprite(stoneTexture);

	sf::Texture bombTexture;
	if (!bombTexture.loadFromFile("images/bombImage.png")) {//���� ����� ��� ������ �������
		bombTexture.create(45, 45);
	}
	sf::Sprite bombSprite(bombTexture);
	
	std::ifstream fin;
	std::string line = "1";
	std::string musStatus = "1";
	std::string soundStatus = "1";
	std::string difficulty = "1";

	fin.open("settings.txt");
	if (fin.is_open()) {//if the file is open
		getline(fin, line);// ������� ������ �� �����
		getline(fin, line);// ������� ������ �� �����
		getline(fin, musStatus);// ������� ������ �� �����
		getline(fin, soundStatus);// ������� ������ �� �����
		getline(fin, difficulty);// ������� ������ �� �����
	}
	fin.close();
	Player hero;
	hero.start(map.positionHeroX, map.positionHeroY, line);
	

	sf::View view = window.getView();//�������� sfml ������ "���", ������� � �������� �������
	

	//������ �� �������, ��������� ��� ���������� ������.
	setView(window, view, hero.heroSprite.getPosition().x + 62, hero.heroSprite.getPosition().y + 80.5, map.TileMapX, map.TileMapY);
	textM.setPosition(view.getCenter().x - 339.5841, view.getCenter().y + 206);//����������� ����� � ������
	//112, 130.5

	sf::Texture cursorTexture;
	cursorTexture.loadFromFile("images/cursorImage.png");
	sf::Sprite cursor(cursorTexture);
	sf::View fixed = window.getView(); // Create a fixed view

	sf::Clock clock; //������� ���������� �������, 


	sf::Texture lazTexture;
	lazTexture.loadFromFile("images/lazerImage.png");
	sf::Sprite lazSprite(lazTexture);
	

	while (status)
	{
		float time = clock.getElapsedTime().asMicroseconds(); //���� ��������� ����� � �������������
		clock.restart(); //������������� �����
		time = time / 800; //�������� ����
		window.clear();//��������� �����

		if (selectedText != -1) {//���� �����-�� ����� ��� �������
			setColor(color, POISONOUS_GREEN);
			textM.setFillColor(color);//��������� �����
			textM.setCharacterSize(50);//������� ������
			textM.setPosition(textM.getPosition().x + 4, textM.getPosition().y);//������� ��������� �������
		}

		sf::Event event;


		short posPortalX = -32768;
		short posPortalY = -32768;
		
		while (window.pollEvent(event))
		{
			selectedText = -1;
			pressedText = -1;
			if (event.type == sf::Event::Closed) status = false;
			sf::Vector2f pos;
			pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));//��������� ����
			if (textM.getGlobalBounds().contains(pos.x - window.getSize().x / 2 + view.getCenter().x, pos.y - window.getSize().y / 2 + view.getCenter().y)) {//���� ���� �� ������
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) pressedText = 1;//���� ������ ����� ������ ���������
				selectedText = 1;//�������� �����
			}
			else {
				if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
					posPortalX = cursor.getPosition().x;
					posPortalY = cursor.getPosition().y;
				}
			}
		}

		if (posPortalX != -32768) {
			hero.portal.setPortal(map.TileMap, posPortalX, posPortalY);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			hero.updateCurrentFrame(0.005*time);
			//0 ���� 1 ��� 2 ���� 3 �����
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { //��� �����
				hero.updateFrame(1, (short)hero.CurrentFrame);
				hero.addPosition(map, -0.1*time, 0);
				hero.checkPortal(map, 2);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { //��� ������
				hero.updateFrame(2, (short)hero.CurrentFrame);
				hero.addPosition(map, 0.1*time, 0);
				hero.checkPortal(map, 3);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { //��� �����
				hero.updateFrame(3, (short)hero.CurrentFrame);
				hero.addPosition(map, 0, -0.1*time);
				hero.checkPortal(map, 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {//��� ����
				hero.updateFrame(0, (short)hero.CurrentFrame);
				hero.addPosition(map, 0, 0.1*time);
				hero.checkPortal(map, 1);
			}
			 
			if (hero.lvl) { //���� ������� �������
				position++;
				status = false;
			}

			//������ �� �������, ��������� ��� ���������� ������.
			setView(window, view, hero.heroSprite.getPosition().x + 62, hero.heroSprite.getPosition().y + 80.5, map.TileMapX, map.TileMapY);
			
			textM.setPosition(view.getCenter().x - 339.5841, view.getCenter().y + 206);//����������� ����� � ������
		}
		else {
			hero.CurrentFrame = 0;
			hero.updateFrame(0, 0);
		}


		if (selectedText != -1) {//���� �� ���-�� ��������
			if (pressedText == -1) {
				setColor(color, ORANGE);
				textM.setFillColor(color);//��������� �����
			}
			else {//���� �� ���-�� ������
				status = false;
				position = 0;
				textM.setFillColor(sf::Color::Black);
			}
			textM.setCharacterSize(54);//��������� �����
			textM.setPosition(textM.getPosition().x - 4, textM.getPosition().y);//�������� ����-���� ����� ����� ����� ���������� �� ������
		}

		//window.draw(fon);//������������ �� ������ ������
		for (int i = 0; i < map.TileMapY; i++)
		{
			for (int j = 0; j < map.TileMapX; j++)
			{
				//difficulty
				if (difficulty == "1" || (pow2(i * 45 + 22.5 - hero.heroSprite.getPosition().y - 80.5) + pow2(j * 45 + 22.5 - hero.heroSprite.getPosition().x - 62) <= 250*250)) {
					fonSprite.setPosition(j * 45, i * 45);//������� ��������� ��������� �����
					if (map.TileMapFon[i][j] == '0')  fonSprite.setTextureRect(sf::IntRect(2, 390, 45, 45)); //���� ��������� ������ 0, �� ������ �����
					else if (map.TileMapFon[i][j] == '1') fonSprite.setTextureRect(sf::IntRect(50, 406, 45, 45));//���� ��������� ������ 1, �� ������ �����
					else if (map.TileMap[i][j] == '-') fonSprite.setTextureRect(sf::IntRect(581, 406, 45, 45));//���� ��������� ������ -, �� ������ �����
					window.draw(fonSprite);//������ ���������� �� �����

					if (map.TileMap[i][j] == 'k') {
						stoneSprite.setPosition(j * 45, i * 45);//������� ��������� ��������� �����
						window.draw(stoneSprite);//������ ���������� �� �����
					}
					else if (map.TileMap[i][j] == 'm') {
						bombSprite.setPosition(j * 45, i * 45);//������� ��������� ��������� �����
						window.draw(bombSprite);//������ ���������� �� �����
					}
					else if (map.TileMap[i][j] == 'w') {
						lazSprite.setPosition(j * 45, i * 45);//������� ��������� ��������� �����
						lazSprite.setTextureRect(sf::IntRect(45, 0, 45, 45));
						window.draw(lazSprite);//������ ���������� �� �����
					}
					else if (map.TileMap[i][j] == 'q') {
						lazSprite.setPosition(j * 45, i * 45);//������� ��������� ��������� �����
						lazSprite.setTextureRect(sf::IntRect(0, 0, 45, 45));
						window.draw(lazSprite);//������ ���������� �� �����
					}
					else if (map.TileMap[i][j] == 'e') {
						lazSprite.setPosition(j * 45, i * 45);//������� ��������� ��������� �����
						lazSprite.setTextureRect(sf::IntRect(45, 0, -45, 45));
						window.draw(lazSprite);//������ ���������� �� �����
					}
					else if (map.TileMap[i][j] == 'a') {
						lazSprite.setTextureRect(sf::IntRect(0, 0, 45, 45));
						lazSprite.setRotation(90);
						lazSprite.setPosition(j * 45 + 45, i * 45);//������� ��������� ��������� �����
						window.draw(lazSprite);//������ ���������� �� �����
						lazSprite.setRotation(0);
					}
					else if (map.TileMap[i][j] == 's') {
						lazSprite.setTextureRect(sf::IntRect(45, 0, 45, 45));
						lazSprite.setRotation(90);
						lazSprite.setPosition(j * 45 + 45, i * 45);//������� ��������� ��������� �����
						window.draw(lazSprite);//������ ���������� �� �����
						lazSprite.setRotation(0);
					}
					else if (map.TileMap[i][j] == 'd') {
						lazSprite.setTextureRect(sf::IntRect(45, 0, -45, 45));
						lazSprite.setRotation(90);
						lazSprite.setPosition(j * 45 + 45, i * 45);//������� ��������� ��������� �����
						window.draw(lazSprite);//������ ���������� �� �����
						lazSprite.setRotation(0);
					}
				}				
			}
		}



		window.draw(textM);
		for (int i = 0; i < 2; i++)
		{
			if (hero.portal.position[i][0] != -32768) {
				hero.portal.portalSprite.setPosition(hero.portal.position[i][0], hero.portal.position[i][1]);
				window.draw(hero.portal.portalSprite);	
			}
		}
		window.draw(hero.heroSprite);

		sf::Vector2f positionMouse = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
		positionMouse.x -= 512 - view.getCenter().x;
		positionMouse.y -= 384 - view.getCenter().y;
		cursor.setPosition(positionMouse);
		window.draw(cursor);
		window.setView(view);
		
		window.display();//�������� �����
	}
	window.setView(fixed);
}

void printAuthor(sf::RenderWindow &window, short &position) {//��������� �� ������
	sf::Color color;

	//�������� �����������
	sf::Texture fonTexture;
	if (!fonTexture.loadFromFile("images/authorImage.jpg")) {
		fonTexture.create(1024, 768);
	}
	sf::Sprite fon(fonTexture);

	//����� � �����
	sf::Font font;//����� 
	font.loadFromFile("font.ttf");//�������� ������ ������ ���� ������

	sf::Text textM;
	textM = sf::Text("Back", font, 50);
	//textM.setStyle(sf::Text::Bold);
	setColor(color, POISONOUS_GREEN);
	textM.setFillColor(color);//��������� �����. ���� ������ ��� ������, �� �� ��������� �� �����
	textM.setPosition(98.9516, 590);//������ ������� ������

	bool status = true;

	short selectedText = -1;
	short pressedText = -1;

	sf::Texture cursorTexture;
	cursorTexture.loadFromFile("images/cursorImage.png");
	sf::Sprite cursor(cursorTexture);
	sf::View fixed = window.getView(); // Create a fixed view

	while (status)
	{
		window.clear();//��������� �����

		if (selectedText != -1) {//���� �����-�� ����� ��� �������
			setColor(color, POISONOUS_GREEN);
			textM.setFillColor(color);//��������� �����
			textM.setCharacterSize(50);//������� ������
			textM.setPosition(textM.getPosition().x + 4, textM.getPosition().y);//������� ��������� �������

		}


		sf::Event event;
		while (window.pollEvent(event))
		{
			selectedText = -1;
			pressedText = -1;
			if (event.type == sf::Event::Closed) status = false;

			sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));//��������� ����
			if (textM.getGlobalBounds().contains(pos.x, pos.y)) {//���� ���� �� ������
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) pressedText = 1;//���� ������ ����� ������ ���������
				selectedText = 1;//�������� �����
			}
		}

		if (selectedText != -1) {//���� �� ���-�� ��������
			if (pressedText == -1) {
				setColor(color, ORANGE);
				textM.setFillColor(color);//��������� �����
			}
			else {//���� �� ���-�� ������
				status = false;
				position = 0;
				textM.setFillColor(sf::Color::Black);
			}
			textM.setCharacterSize(54);//��������� �����
			textM.setPosition(textM.getPosition().x - 4, textM.getPosition().y);//�������� ����-���� ����� ����� ����� ���������� �� ������
		}

		window.draw(fon);//������������ �� ������ ������

		window.draw(textM);

		cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
		window.draw(cursor);
		window.setView(fixed);

		window.display();//�������� �����
	}
}

void printEndGame(sf::RenderWindow &window, short &position) {//��������� ����� ����
	sf::Color color;

	//�������� �����������
	sf::Texture fonTexture;
	if (!fonTexture.loadFromFile("images/endGameImage.jpg")) {
		fonTexture.create(1024, 768);
	}
	sf::Sprite fon(fonTexture);

	//����� � �����
	sf::Font font;//����� 
	font.loadFromFile("font.ttf");//�������� ������ ������ ���� ������

	sf::Text textM;
	textM = sf::Text("Back", font, 50);
	//textM.setStyle(sf::Text::Bold);
	setColor(color, POISONOUS_GREEN);
	textM.setFillColor(color);//��������� �����. ���� ������ ��� ������, �� �� ��������� �� �����
	textM.setPosition(98.9516, 590);//������ ������� ������

	bool status = true;

	short selectedText = -1;
	short pressedText = -1;

	sf::Texture cursorTexture;
	cursorTexture.loadFromFile("images/cursorImage.png");
	sf::Sprite cursor(cursorTexture);
	sf::View fixed = window.getView(); // Create a fixed view

	while (status)
	{
		window.clear();//��������� �����

		if (selectedText != -1) {//���� �����-�� ����� ��� �������
			setColor(color, POISONOUS_GREEN);
			textM.setFillColor(color);//��������� �����
			textM.setCharacterSize(50);//������� ������
			textM.setPosition(textM.getPosition().x + 4, textM.getPosition().y);//������� ��������� �������

		}


		sf::Event event;
		while (window.pollEvent(event))
		{
			selectedText = -1;
			pressedText = -1;
			if (event.type == sf::Event::Closed) status = false;

			sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));//��������� ����
			if (textM.getGlobalBounds().contains(pos.x, pos.y)) {//���� ���� �� ������
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) pressedText = 1;//���� ������ ����� ������ ���������
				selectedText = 1;//�������� �����
			}
		}

		if (selectedText != -1) {//���� �� ���-�� ��������
			if (pressedText == -1) {
				setColor(color, ORANGE);
				textM.setFillColor(color);//��������� �����
			}
			else {//���� �� ���-�� ������
				status = false;
				position = 0;
				textM.setFillColor(sf::Color::Black);
			}
			textM.setCharacterSize(54);//��������� �����
			textM.setPosition(textM.getPosition().x - 4, textM.getPosition().y);//�������� ����-���� ����� ����� ����� ���������� �� ������
		}

		window.draw(fon);//������������ �� ������ ������

		window.draw(textM);

		cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
		window.draw(cursor);
		window.setView(fixed);

		window.display();//�������� �����
	}
}

void printError(sf::RenderWindow &window, short &position) {//��������� �������������
	sf::Color color;

	//�������� �����������
	sf::Texture fonTexture;
	if (!fonTexture.loadFromFile("images/errorImage.jpg")) {
		fonTexture.create(1024, 768);
	}
	sf::Sprite fon(fonTexture);

	//����� � �����
	sf::Font font;//����� 
	font.loadFromFile("font.ttf");//�������� ������ ������ ���� ������

	sf::Text textM;
	textM = sf::Text("Back", font, 50);
	//textM.setStyle(sf::Text::Bold);
	setColor(color, POISONOUS_GREEN);
	textM.setFillColor(color);//��������� �����. ���� ������ ��� ������, �� �� ��������� �� �����
	textM.setPosition(98.9516, 590);//������ ������� ������

	bool status = true;

	short selectedText = -1;
	short pressedText = -1;

	sf::Texture cursorTexture;
	cursorTexture.loadFromFile("images/cursorImage.png");
	sf::Sprite cursor(cursorTexture);
	sf::View fixed = window.getView(); // Create a fixed view

	while (status)
	{
		window.clear();//��������� �����

		if (selectedText != -1) {//���� �����-�� ����� ��� �������
			setColor(color, POISONOUS_GREEN);
			textM.setFillColor(color);//��������� �����
			textM.setCharacterSize(50);//������� ������
			textM.setPosition(textM.getPosition().x + 4, textM.getPosition().y);//������� ��������� �������

		}


		sf::Event event;
		while (window.pollEvent(event))
		{
			selectedText = -1;
			pressedText = -1;
			if (event.type == sf::Event::Closed) status = false;

			sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));//��������� ����
			if (textM.getGlobalBounds().contains(pos.x, pos.y)) {//���� ���� �� ������
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) pressedText = 1;//���� ������ ����� ������ ���������
				selectedText = 1;//�������� �����
			}
		}

		if (selectedText != -1) {//���� �� ���-�� ��������
			if (pressedText == -1) {
				setColor(color, ORANGE);
				textM.setFillColor(color);//��������� �����
			}
			else {//���� �� ���-�� ������
				status = false;
				position = 0;
				textM.setFillColor(sf::Color::Black);
			}
			textM.setCharacterSize(54);//��������� �����
			textM.setPosition(textM.getPosition().x - 4, textM.getPosition().y);//�������� ����-���� ����� ����� ����� ���������� �� ������
		}

		window.draw(fon);//������������ �� ������ ������

		window.draw(textM);

		cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
		window.draw(cursor);
		window.setView(fixed);

		window.display();//�������� �����
	}
}

void printSettings(sf::RenderWindow &window, short &position) {//show settings
	sf::Color color;

	//�������� �����������
	sf::Texture fonTexture;
	if (!fonTexture.loadFromFile("images/settingsImage.jpg")) {
		fonTexture.create(1024, 768);
	}
	sf::Sprite fon(fonTexture);

	//����� � �����
	sf::Font font;//����� 
	font.loadFromFile("font.ttf");//�������� ������ ������ ���� ������

	sf::Text textM;
	textM = sf::Text("Back", font, 50);
	//textM.setStyle(sf::Text::Bold);
	setColor(color, POISONOUS_GREEN);
	textM.setFillColor(color);//��������� �����. ���� ������ ��� ������, �� �� ��������� �� �����
	textM.setPosition(98.9516, 590);//������ ������� ������

	bool status = true;

	short selectedText = -1;
	short pressedText = -1;

	sf::Texture cursorTexture;
	cursorTexture.loadFromFile("images/cursorImage.png");
	sf::Sprite cursor(cursorTexture);
	sf::View fixed = window.getView(); // Create a fixed view


	std::ifstream fin;
	std::string line = "1";
	std::string musStatus = "1";
	std::string soundStatus = "1";
	std::string difficulty = "1";

	fin.open("settings.txt");
	if (fin.is_open()) {//if the file is open
		getline(fin, line);// ������� ������ �� �����
		getline(fin, line);// ������� ������ �� �����
		getline(fin, musStatus);// ������� ������ �� �����
		getline(fin, soundStatus);// ������� ������ �� �����
		getline(fin, difficulty);// ������� ������ �� �����
	}
	fin.close();
	Player hero;
	hero.start(100, 190, line);

	sf::Texture arrowsTexture;
	if (!arrowsTexture.loadFromFile("images/arrows.png")) {
		arrowsTexture.create(90, 64);
	}
	sf::Sprite arrow1Sprite(arrowsTexture);
	sf::Sprite arrow2Sprite(arrowsTexture);
	arrow2Sprite.setPosition(177, 360);
	
	arrow1Sprite.setTextureRect(sf::IntRect(90, 0, -90, 64));
	arrow1Sprite.setPosition(57, 360);


	sf::Clock clock; //������� ���������� �������
	while (status)
	{
		float time = clock.getElapsedTime().asMicroseconds(); //���� ��������� ����� � �������������
		clock.restart(); //������������� �����
		time = time / 800; //�������� ����
		window.clear();//��������� �����

		if (selectedText != -1) {//���� �����-�� ����� ��� �������
			setColor(color, POISONOUS_GREEN);
			textM.setFillColor(color);//��������� �����
			textM.setCharacterSize(50);//������� ������
			textM.setPosition(textM.getPosition().x + 4, textM.getPosition().y);//������� ��������� �������

		}


		sf::Event event;
		while (window.pollEvent(event))
		{
			selectedText = -1;
			pressedText = -1;
			if (event.type == sf::Event::Closed) status = false;

			sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));//��������� ����
			if (textM.getGlobalBounds().contains(pos.x, pos.y)) {//���� ���� �� ������
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) pressedText = 1;//���� ������ ����� ������ ���������
				selectedText = 1;//�������� �����
			}
			if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				if (arrow1Sprite.getGlobalBounds().contains(pos.x, pos.y)) {//���� ���� �� �������
					int number = atoi(line.c_str());
					number--;
					if (number < 1) number = 5;
					line = std::to_string(number);
					hero.start(100, 190, line);
				}
				if (arrow2Sprite.getGlobalBounds().contains(pos.x, pos.y)) {//���� ���� �� �������
					int number = atoi(line.c_str());
					number++;
					if (number > 5) number = 1;
					line = std::to_string(number);
					hero.start(100, 190, line);
				}
			}
			
			
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			hero.updateCurrentFrame(0.005*time);
			//0 ���� 1 ��� 2 ���� 3 �����
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { //��� �����
				hero.updateFrame(1, (short)hero.CurrentFrame);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { //��� ������
				hero.updateFrame(2, (short)hero.CurrentFrame);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { //��� �����
				hero.updateFrame(3, (short)hero.CurrentFrame);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {//��� ����
				hero.updateFrame(0, (short)hero.CurrentFrame);
			}

		}
		else {
			hero.CurrentFrame = 0;
			hero.updateFrame(0, 0);
		}

		if (selectedText != -1) {//���� �� ���-�� ��������
			if (pressedText == -1) {
				setColor(color, ORANGE);
				textM.setFillColor(color);//��������� �����
			}
			else {//���� �� ���-�� ������
				status = false;
				position = 0;
				textM.setFillColor(sf::Color::Black);
				std::ofstream out("settings.txt");
				out << "1.1" << std::endl;
				out << line << std::endl;
				out << musStatus << std::endl;
				out << soundStatus << std::endl;
				out << difficulty;
				out.close();
			}
			textM.setCharacterSize(54);//��������� �����
			textM.setPosition(textM.getPosition().x - 4, textM.getPosition().y);//�������� ����-���� ����� ����� ����� ���������� �� ������
		}

		window.draw(fon);//������������ �� ������ ���
		window.draw(arrow1Sprite);
		window.draw(arrow2Sprite);
		window.draw(hero.heroSprite);
		window.draw(textM);

		cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
		window.draw(cursor);
		window.setView(fixed);

		window.display();//�������� �����
	}
}

void printMenu(sf::RenderWindow &window, short &position) {//��������� ����
	sf::Color color;
	setColor(color, POISONOUS_GREEN);
	//�������� �����������
	sf::Texture fonTexture;
	if (!fonTexture.loadFromFile("images/menuImage.jpg")) {
		fonTexture.create(1024, 768);
	}
	sf::Sprite fon(fonTexture);
	//fonTexture.~Texture();//������� �������� ��� ��� ��� ������ �� �����, �� ��� ������ � ������
	//����� � �����
	sf::Font font;//����� 
	font.loadFromFile("font.ttf");//�������� ������ ������ ���� ������
	sf::Text textM[5];
	for (int i = 0; i < 4; i++)
	{
		textM[i] = sf::Text("", font, 50);
		//textM[i].setStyle(sf::Text::Bold);
		textM[i].setFillColor(color);//��������� �����. ���� ������ ��� ������, �� �� ��������� �� �����

	}
	//�� 256 �� ������
	textM[0].setString("Play");//������ ������ ������
	textM[0].setPosition(100, 30);//������ ������� ������ 
	textM[1].setString("Options");//������ ������ ������
	textM[1].setPosition(290, 30);//������ ������� ������
	textM[2].setString("Author");//������ ������ ������
	textM[2].setPosition(560, 30);//������ ������� ������
	textM[3].setString("Exit");//������ ������ ������
	textM[3].setPosition(820, 30);//������ ������� ������


	bool status = true;

	short selectedText = -1;
	short pressedText = -1;

	sf::Texture cursorTexture;
	cursorTexture.loadFromFile("images/cursorImage.png");
	sf::Sprite cursor(cursorTexture);
	sf::View fixed = window.getView(); // Create a fixed view

	while (status)
	{
		window.clear();//��������� �����

		if (selectedText != -1) {//���� �����-�� ����� ��� �������
			setColor(color, POISONOUS_GREEN);
			textM[selectedText].setFillColor(color);//��������� �����
			textM[selectedText].setCharacterSize(50);//������� ������
			textM[selectedText].setPosition(textM[selectedText].getPosition().x + 4, textM[selectedText].getPosition().y);//������� ��������� �������

		}


		sf::Event event;
		while (window.pollEvent(event))
		{
			selectedText = -1;
			pressedText = -1;
			if (event.type == sf::Event::Closed) status = false;
			//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) status = false;
			//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Home)) ::SendNotifyMessage(window.getSystemHandle(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
			sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));//��������� ����
			for (int i = 0; i < 4; i++)
			{
				if (textM[i].getGlobalBounds().contains(pos.x, pos.y)) {//���� ���� �� ������
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) pressedText = i;//���� ������ ����� ������ ���������

					selectedText = i;//�������� �����
				}
			}
			//���� ������ �� ������
			if ((pow2(pos.x - 931) / pow2(70) + pow2(pos.y - 438) / pow2(305.5)) <= 1) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					status = false;
					position = -2;
				}
			}
		}

		if (selectedText != -1) {//���� �� ���-�� ��������
			setColor(color, ORANGE);
			if (pressedText == -1) {
				textM[selectedText].setFillColor(color);//��������� �����
			}
			else {
				textM[selectedText].setFillColor(sf::Color::Black);//��������� �����
				if (pressedText == 0) {//���� ������ ����� ����
					position = 4;
				}
				else if (pressedText == 1) {//���� ������ ���������
					position = 2;
				}
				else if (pressedText == 2) {//���� ������ �� ������
					position = 1;
				}
				else if (pressedText == 3) {//���� ������ �����
					position = -1;
				}
				status = false;
			}

			textM[selectedText].setCharacterSize(54);//��������� �����
			textM[selectedText].setPosition(textM[selectedText].getPosition().x - 4, textM[selectedText].getPosition().y);//�������� ����-���� ����� ����� ����� ���������� �� ������
		}

		window.draw(fon);//������������ �� ������ ������
		for (int i = 0; i < 4; i++)
		{
			window.draw(textM[i]);
		}

		cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
		window.draw(cursor);
		window.setView(fixed);

		window.display();//�������� �����
	}
}

void launch(sf::RenderWindow &window, short &position) {//��������� ����
	sf::Texture fonTexture;
	if (!fonTexture.loadFromFile("images/loadingImage.jpg")) {
		fonTexture.create(1024, 768);
	}
	sf::Sprite fon(fonTexture);
	
	std::ifstream fin;
	std::string line = "1";
	fin.open("settings.txt");
	if (fin.is_open()) {//if the file is open
		getline(fin, line);// ������� ������ �� �����
		getline(fin, line);// ������� ������ �� �����
		getline(fin, line);// ������� ������ �� �����
	}
	fin.close();

	sf::Music music;
	if (line == "1") {
		music.openFromFile("music/backgroundMusic.ogg");
		music.play();
		music.setLoop(true);
	}
	/*
	//������ ������
	sf::Music music;
	music.openFromFile("music/backgroundMusic.ogg");
	music.play();
	music.setLoop(true);
	bool musicStatus = true;
	bool shootStatus = false;
	//���� ���������
	sf::SoundBuffer shootBuffer;//������ ����� ��� �����
	shootBuffer.loadFromFile("music/gameOver.ogg");//��������� � ���� ����
	sf::Sound shoot(shootBuffer);//������� ���� � ��������� � ���� ���� �� ������
	*/

	while (position != -1) {
		if (position == 0)
		{
			printMenu(window, position);
		}
		else if (position == 1)
		{
			printAuthor(window, position);
		}
		else if (position == -2)
		{
			printError(window, position);
		}
		else if (position == 2)
		{
			printSettings(window, position);
		}
		else if (position == 3)
		{
			printEndGame(window, position);
		}
		else if (position > 3)
		{
			printGame(window, position);
		}
		if (position != -1) {
			window.clear();
			window.draw(fon);//������������ �� ������ ������
			window.display();//�������� �����
		}

	}
}
