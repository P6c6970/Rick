#pragma once
#include <iostream>

#include "def.h"
#include "map.h"//карта
#include "hero.h"//игрок и портал

void printGame(sf::RenderWindow &window, short &position, sf::Sprite &cursor, sf::Music &music) {//Отрисовка уровней
	sf::Color color;
	//Текст и шрифт
	sf::Font font;//шрифт 
	font.loadFromFile("font.ttf");//передаем нашему шрифту файл шрифта

	sf::Text textM;
	textM = sf::Text("Back", font, 50);
	//textM.setStyle(sf::Text::Bold);
	setColor(color, POISONOUS_GREEN);
	textM.setFillColor(color);//покрасили текст. если убрать эту строку, то по умолчанию он белый
	
	bool status = true;

	short selectedText = -1;
	short pressedText = -1;

	Map map;
	map.load(status, position - 3);
	if (status == false) position = 3;

	sf::Texture stoneTexture;
	sf::Sprite stoneSprite;
	createTexture(stoneTexture, stoneSprite, "images/stoneImage.png", 45, 45);

	sf::Texture bombTexture;
	sf::Sprite bombSprite;
	createTexture(bombTexture, bombSprite, "images/bombImage.png", 45, 45);
	
	sf::Texture lazTexture;
	sf::Sprite lazSprite;
	createTexture(lazTexture, lazSprite, "images/lazerImage.png", 90, 45);

	std::ifstream fin;
	std::string line = "1";
	std::string musStatus = "1";
	std::string soundStatus = "1";
	std::string difficulty = "1";

	fin.open("settings.txt");
	if (fin.is_open()) {//if the file is open
		getline(fin, line);// считали строку из файла
		getline(fin, line);// считали строку из файла
		getline(fin, musStatus);// считали строку из файла
		getline(fin, soundStatus);// считали строку из файла
		getline(fin, difficulty);// считали строку из файла
	}
	fin.close();
	Player hero;
	hero.start(map.positionHeroX, map.positionHeroY, line);
	
	sf::SoundBuffer gameOverBuff;//создаём буфер для звука
	gameOverBuff.loadFromFile("music/gameOver.ogg");//загружаем в него звук
	sf::Sound gameOverS(gameOverBuff);//создаем звук и загружаем в него звук из буфера
	sf::SoundBuffer mistakeBuff;//создаём буфер для звука
	mistakeBuff.loadFromFile("music/mistake.ogg");//загружаем в него звук
	sf::Sound mistakeS(mistakeBuff);//создаем звук и загружаем в него звук из буфера
	
	sf::View view = window.getView();//объявили sfml объект "вид", который и является камерой
	//следим за игроком, передавая его координаты камере.
	setView(window, view, hero.heroSprite.getPosition().x + 62, hero.heroSprite.getPosition().y + 80.5, map.TileMapX, map.TileMapY);
	textM.setPosition(view.getCenter().x - 339.5841, view.getCenter().y + 206);//привязываем текст к камере
	//112, 130.5

	
	sf::View fixed = window.getView(); // Create a fixed view

	sf::Clock clock; //создаем переменную времени, 

	while (status)
	{
		float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время
		time = time / 800; //скорость игры
		window.clear();//отчисщаем экран

		if (selectedText != -1) {//если какой-то текст был выделен
			setColor(color, POISONOUS_GREEN);
			textM.setFillColor(color);//покрасили текст
			textM.setCharacterSize(50);//вернули кегель
			textM.setPosition(textM.getPosition().x + 4, textM.getPosition().y);//вернули начальную позицию
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
			pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));//положение мыши
			if (textM.getGlobalBounds().contains(pos.x - window.getSize().x / 2 + view.getCenter().x, pos.y - window.getSize().y / 2 + view.getCenter().y)) {//если мышь на тексте
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) pressedText = 1;//если нажата левая кнопка выполняем
				selectedText = 1;//выделяем текст
			}
			else {
				if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
					posPortalX = cursor.getPosition().x;
					posPortalY = cursor.getPosition().y;
				}
			}
		}

		if (posPortalX != -32768) {
			if (hero.portal.setPortal(map.TileMap, posPortalX, posPortalY)) {
				soundGameOverS(soundStatus, mistakeS);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			hero.updateCurrentFrame(0.005*time);
			//0 верх 1 низ 2 лево 3 право
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { //идём влево
				hero.updateFrame(1, (short)hero.CurrentFrame);
				if (hero.addPosition(map, -0.1*time, 0)) {
					soundGameOverS(soundStatus, gameOverS);
				}
				hero.checkPortal(map, 2);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { //идём вправо
				hero.updateFrame(2, (short)hero.CurrentFrame);
				if (hero.addPosition(map, 0.1*time, 0)) {
					soundGameOverS(soundStatus, gameOverS);
				}
				hero.checkPortal(map, 3);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { //идём вверх
				hero.updateFrame(3, (short)hero.CurrentFrame);
				if (hero.addPosition(map, 0, -0.1*time)) {
					soundGameOverS(soundStatus, gameOverS);
				}
				hero.checkPortal(map, 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {//идём вниз
				hero.updateFrame(0, (short)hero.CurrentFrame);
				if (hero.addPosition(map, 0, 0.1*time)) {
					soundGameOverS(soundStatus, gameOverS);
				}
				hero.checkPortal(map, 1);
			}
			 
			if (hero.lvl) { //если уровень пройдет
				position++;
				status = false;
			}

			//следим за игроком, передавая его координаты камере.
			setView(window, view, hero.heroSprite.getPosition().x + 62, hero.heroSprite.getPosition().y + 80.5, map.TileMapX, map.TileMapY);
			
			textM.setPosition(view.getCenter().x - 339.5841, view.getCenter().y + 206);//привязываем текст к камере
		}
		else {
			hero.CurrentFrame = 0;
			hero.updateFrame(0, 0);
		}

		if (selectedText != -1) {//если на что-то наведено
			if (pressedText == -1) {
				setColor(color, ORANGE);
				textM.setFillColor(color);//покрасили текст
			}
			else {//если на что-то нажато
				status = false;
				position = 0;
				textM.setFillColor(sf::Color::Black);
			}
			textM.setCharacterSize(54);//увеличели кегль
			textM.setPosition(textM.getPosition().x - 4, textM.getPosition().y);//сдвигаем чуть-чуть влево чтобы текст увеличился из центра
		}

		//window.draw(fon);//отрисовываем на экране тексты
		for (int i = 0; i < map.TileMapY; i++)
		{
			for (int j = 0; j < map.TileMapX; j++)
			{
				//difficulty
				if (difficulty == "1" || (pow2(i * 45 + 22.5 - hero.heroSprite.getPosition().y - 80.5) + pow2(j * 45 + 22.5 - hero.heroSprite.getPosition().x - 62) <= 250*250)) {
					map.fonSprite.setPosition(j * 45, i * 45);//считаем положение элементов карты
					if (map.TileMapFon[i][j] == '0')  map.fonSprite.setTextureRect(sf::IntRect(2, 390, 45, 45)); //если встретили символ 0, то рисуем землю
					else if (map.TileMapFon[i][j] == '1') map.fonSprite.setTextureRect(sf::IntRect(50, 406, 45, 45));//если встретили символ 1, то рисуем землю
					else if (map.TileMap[i][j] == '-') map.fonSprite.setTextureRect(sf::IntRect(581, 406, 45, 45));//если встретили символ -, то рисуем стену
					window.draw(map.fonSprite);//рисуем квадратики на экран

					if (map.TileMap[i][j] == 'k') {
						stoneSprite.setPosition(j * 45, i * 45);//считаем положение элементов карты
						window.draw(stoneSprite);//рисуем квадратики на экран
					}
					else if (map.TileMap[i][j] == 'm') {
						bombSprite.setPosition(j * 45, i * 45);//считаем положение элементов карты
						window.draw(bombSprite);//рисуем квадратики на экран
					}
					else if (map.TileMap[i][j] == 'w') {
						lazSprite.setPosition(j * 45, i * 45);//считаем положение элементов карты
						lazSprite.setTextureRect(sf::IntRect(45, 0, 45, 45));
						window.draw(lazSprite);//рисуем квадратики на экран
					}
					else if (map.TileMap[i][j] == 'q') {
						lazSprite.setPosition(j * 45, i * 45);//считаем положение элементов карты
						lazSprite.setTextureRect(sf::IntRect(0, 0, 45, 45));
						window.draw(lazSprite);//рисуем квадратики на экран
					}
					else if (map.TileMap[i][j] == 'e') {
						lazSprite.setPosition(j * 45, i * 45);//считаем положение элементов карты
						lazSprite.setTextureRect(sf::IntRect(45, 0, -45, 45));
						window.draw(lazSprite);//рисуем квадратики на экран
					}
					else if (map.TileMap[i][j] == 'a') {
						lazSprite.setTextureRect(sf::IntRect(0, 0, 45, 45));
						lazSprite.setRotation(90);
						lazSprite.setPosition(j * 45 + 45, i * 45);//считаем положение элементов карты
						window.draw(lazSprite);//рисуем квадратики на экран
						lazSprite.setRotation(0);
					}
					else if (map.TileMap[i][j] == 's') {
						lazSprite.setTextureRect(sf::IntRect(45, 0, 45, 45));
						lazSprite.setRotation(90);
						lazSprite.setPosition(j * 45 + 45, i * 45);//считаем положение элементов карты
						window.draw(lazSprite);//рисуем квадратики на экран
						lazSprite.setRotation(0);
					}
					else if (map.TileMap[i][j] == 'd') {
						lazSprite.setTextureRect(sf::IntRect(45, 0, -45, 45));
						lazSprite.setRotation(90);
						lazSprite.setPosition(j * 45 + 45, i * 45);//считаем положение элементов карты
						window.draw(lazSprite);//рисуем квадратики на экран
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
		
		window.display();//обновить экран
	}
	window.setView(fixed);
}

void printAuthor(sf::RenderWindow &window, short &position, sf::Sprite &cursor) {//Отрисовка об авторе
	sf::Color color;

	sf::Texture fonTexture;
	sf::Sprite fon;
	createTexture(fonTexture, fon, "images/authorImage.jpg", 1024, 768);

	//Текст и шрифт
	sf::Font font;//шрифт 
	font.loadFromFile("font.ttf");//передаем нашему шрифту файл шрифта

	sf::Text textM;
	textM = sf::Text("Back", font, 50);
	//textM.setStyle(sf::Text::Bold);
	setColor(color, POISONOUS_GREEN);
	textM.setFillColor(color);//покрасили текст. если убрать эту строку, то по умолчанию он белый
	textM.setPosition(98.9516, 590);//задаем позицию текста

	bool status = true;

	short selectedText = -1;
	short pressedText = -1;

	sf::View fixed = window.getView(); // Create a fixed view

	while (status)
	{
		window.clear();//отчисщаем экран

		if (selectedText != -1) {//если какой-то текст был выделен
			setColor(color, POISONOUS_GREEN);
			textM.setFillColor(color);//покрасили текст
			textM.setCharacterSize(50);//вернули кегель
			textM.setPosition(textM.getPosition().x + 4, textM.getPosition().y);//вернули начальную позицию

		}


		sf::Event event;
		while (window.pollEvent(event))
		{
			selectedText = -1;
			pressedText = -1;
			if (event.type == sf::Event::Closed) status = false;

			sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));//положение мыши
			if (textM.getGlobalBounds().contains(pos.x, pos.y)) {//если мышь на тексте
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) pressedText = 1;//если нажата левая кнопка выполняем
				selectedText = 1;//выделяем текст
			}
		}

		if (selectedText != -1) {//если на что-то наведено
			if (pressedText == -1) {
				setColor(color, ORANGE);
				textM.setFillColor(color);//покрасили текст
			}
			else {//если на что-то нажато
				status = false;
				position = 0;
				textM.setFillColor(sf::Color::Black);
			}
			textM.setCharacterSize(54);//увеличели кегль
			textM.setPosition(textM.getPosition().x - 4, textM.getPosition().y);//сдвигаем чуть-чуть влево чтобы текст увеличился из центра
		}

		window.draw(fon);//отрисовываем на экране тексты

		window.draw(textM);

		cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
		window.draw(cursor);
		window.setView(fixed);

		window.display();//обновить экран
	}
}

void printEndGame(sf::RenderWindow &window, short &position, sf::Sprite &cursor) {//Отрисовка конца игры
	sf::Color color;

	sf::Texture fonTexture;
	sf::Sprite fon;
	createTexture(fonTexture, fon, "images/endGameImage.jpg", 1024, 768);

	//Текст и шрифт
	sf::Font font;//шрифт 
	font.loadFromFile("font.ttf");//передаем нашему шрифту файл шрифта

	sf::Text textM;
	textM = sf::Text("Back", font, 50);
	//textM.setStyle(sf::Text::Bold);
	setColor(color, POISONOUS_GREEN);
	textM.setFillColor(color);//покрасили текст. если убрать эту строку, то по умолчанию он белый
	textM.setPosition(98.9516, 590);//задаем позицию текста

	bool status = true;

	short selectedText = -1;
	short pressedText = -1;

	sf::View fixed = window.getView(); // Create a fixed view

	while (status)
	{
		window.clear();//отчисщаем экран

		if (selectedText != -1) {//если какой-то текст был выделен
			setColor(color, POISONOUS_GREEN);
			textM.setFillColor(color);//покрасили текст
			textM.setCharacterSize(50);//вернули кегель
			textM.setPosition(textM.getPosition().x + 4, textM.getPosition().y);//вернули начальную позицию

		}


		sf::Event event;
		while (window.pollEvent(event))
		{
			selectedText = -1;
			pressedText = -1;
			if (event.type == sf::Event::Closed) status = false;

			sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));//положение мыши
			if (textM.getGlobalBounds().contains(pos.x, pos.y)) {//если мышь на тексте
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) pressedText = 1;//если нажата левая кнопка выполняем
				selectedText = 1;//выделяем текст
			}
		}

		if (selectedText != -1) {//если на что-то наведено
			if (pressedText == -1) {
				setColor(color, ORANGE);
				textM.setFillColor(color);//покрасили текст
			}
			else {//если на что-то нажато
				status = false;
				position = 0;
				textM.setFillColor(sf::Color::Black);
			}
			textM.setCharacterSize(54);//увеличели кегль
			textM.setPosition(textM.getPosition().x - 4, textM.getPosition().y);//сдвигаем чуть-чуть влево чтобы текст увеличился из центра
		}

		window.draw(fon);//отрисовываем на экране тексты

		window.draw(textM);

		cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
		window.draw(cursor);
		window.setView(fixed);

		window.display();//обновить экран
	}
}

void printError(sf::RenderWindow &window, short &position, sf::Sprite &cursor) {//Отрисовка недоделанного
	sf::Color color;

	sf::Texture fonTexture;
	sf::Sprite fon;
	createTexture(fonTexture, fon, "images/errorImage.jpg", 1024, 768);

	//Текст и шрифт
	sf::Font font;//шрифт 
	font.loadFromFile("font.ttf");//передаем нашему шрифту файл шрифта

	sf::Text textM;
	textM = sf::Text("Back", font, 50);
	//textM.setStyle(sf::Text::Bold);
	setColor(color, POISONOUS_GREEN);
	textM.setFillColor(color);//покрасили текст. если убрать эту строку, то по умолчанию он белый
	textM.setPosition(98.9516, 590);//задаем позицию текста

	bool status = true;

	short selectedText = -1;
	short pressedText = -1;

	sf::View fixed = window.getView(); // Create a fixed view

	while (status)
	{
		window.clear();//отчисщаем экран

		if (selectedText != -1) {//если какой-то текст был выделен
			setColor(color, POISONOUS_GREEN);
			textM.setFillColor(color);//покрасили текст
			textM.setCharacterSize(50);//вернули кегель
			textM.setPosition(textM.getPosition().x + 4, textM.getPosition().y);//вернули начальную позицию

		}


		sf::Event event;
		while (window.pollEvent(event))
		{
			selectedText = -1;
			pressedText = -1;
			if (event.type == sf::Event::Closed) status = false;

			sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));//положение мыши
			if (textM.getGlobalBounds().contains(pos.x, pos.y)) {//если мышь на тексте
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) pressedText = 1;//если нажата левая кнопка выполняем
				selectedText = 1;//выделяем текст
			}
		}

		if (selectedText != -1) {//если на что-то наведено
			if (pressedText == -1) {
				setColor(color, ORANGE);
				textM.setFillColor(color);//покрасили текст
			}
			else {//если на что-то нажато
				status = false;
				position = 0;
				textM.setFillColor(sf::Color::Black);
			}
			textM.setCharacterSize(54);//увеличели кегль
			textM.setPosition(textM.getPosition().x - 4, textM.getPosition().y);//сдвигаем чуть-чуть влево чтобы текст увеличился из центра
		}

		window.draw(fon);//отрисовываем на экране тексты

		window.draw(textM);

		cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
		window.draw(cursor);
		window.setView(fixed);

		window.display();//обновить экран
	}
}

void printSettings(sf::RenderWindow &window, short &position, sf::Sprite &cursor, sf::Music &music) {//show settings
	sf::Color color;

	sf::Texture fonTexture;
	sf::Sprite fon;
	createTexture(fonTexture, fon, "images/settingsImage.jpg", 1024, 768);
	//Текст и шрифт
	sf::Font font;//шрифт 
	font.loadFromFile("font.ttf");//передаем нашему шрифту файл шрифта

	sf::Text textM[4];
	for (int i = 0; i < 4; i++)
	{
		textM[i] = sf::Text("", font, 50);
	}
	textM[0].setString("Music");//задает строку тексту
	textM[0].setPosition(650, 250);//задаем позицию текста 
	textM[1].setString("Sounds");//задает строку тексту
	textM[1].setPosition(650, 350);//задаем позицию текста
	textM[2].setString("Difficulty");//задает строку тексту
	textM[2].setPosition(650, 450);//задаем позицию текста
	textM[3].setString("Back");//задает строку тексту
	textM[3].setPosition(98.9516, 590);//задаем позицию текста

	bool status = true;

	short selectedText = -1;
	short pressedText = -1;

	sf::View fixed = window.getView(); // Create a fixed view

	std::ifstream fin;
	std::string line = "1";
	std::string musStatus = "1";
	std::string soundStatus = "1";
	std::string difficulty = "1";

	fin.open("settings.txt");
	if (fin.is_open()) {//if the file is open
		getline(fin, line);// считали строку из файла
		getline(fin, line);// считали строку из файла
		getline(fin, musStatus);// считали строку из файла
		getline(fin, soundStatus);// считали строку из файла
		getline(fin, difficulty);// считали строку из файла
	}
	fin.close();

	if (musStatus=="1") setColor(color, POISONOUS_GREEN);
	else setColor(color, RUBY);
	textM[0].setFillColor(color);//покрасили текст. если убрать эту строку, то по умолчанию он белый
	if (soundStatus == "1") setColor(color, POISONOUS_GREEN);
	else setColor(color, RUBY);
	textM[1].setFillColor(color);//покрасили текст. если убрать эту строку, то по умолчанию он белый
	if (difficulty == "0") setColor(color, POISONOUS_GREEN);
	else setColor(color, RUBY);
	textM[2].setFillColor(color);//покрасили текст. если убрать эту строку, то по умолчанию он белый
	setColor(color, POISONOUS_GREEN);
	textM[3].setFillColor(color);//покрасили текст. если убрать эту строку, то по умолчанию он белый

	Player hero;
	hero.start(100, 190, line);

	sf::Texture arrowsTexture;
	sf::Sprite arrow1Sprite;
	sf::Sprite arrow2Sprite;
	createTexture(arrowsTexture, arrow1Sprite, "images/arrows.png", 90, 64);
	createTexture(arrowsTexture, arrow2Sprite, "images/arrows.png", 90, 64);
	arrow2Sprite.setPosition(177, 360);
	arrow1Sprite.setTextureRect(sf::IntRect(90, 0, -90, 64));
	arrow1Sprite.setPosition(57, 360);

	sf::Clock clock; //создаем переменную времени
	while (status)
	{
		float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время
		time = time / 800; //скорость игры
		window.clear();//отчисщаем экран

		if (selectedText != -1) {//если какой-то текст был выделен
			switch (selectedText)
			{
			case 0:
				if (musStatus == "1") setColor(color, POISONOUS_GREEN);
				else setColor(color, RUBY);
				break;
			case 1:
				if (soundStatus == "1") setColor(color, POISONOUS_GREEN);
				else setColor(color, RUBY);
				break;
			case 2:
				if (difficulty == "0") setColor(color, POISONOUS_GREEN);
				else setColor(color, RUBY);
				break;
			case 3:
				setColor(color, POISONOUS_GREEN);
				break;
			}
			textM[selectedText].setFillColor(color);//покрасили текст
			textM[selectedText].setCharacterSize(50);//вернули кегель
			textM[selectedText].setPosition(textM[selectedText].getPosition().x + 4, textM[selectedText].getPosition().y);//вернули начальную позицию

		}


		sf::Event event;
		while (window.pollEvent(event))
		{
			selectedText = -1;
			pressedText = -1;
			if (event.type == sf::Event::Closed) status = false;

			sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));//положение мыши

			for (int i = 0; i < 4; i++)
			{
				if (textM[i].getGlobalBounds().contains(pos.x, pos.y)) {//если мышь на тексте
					selectedText = i;//выделяем текст
					if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
						pressedText = i;//если нажата левая кнопка выполняем
						if (i == 0) {//musStatus
							if (musStatus == "1") {
								musStatus = "0";
								music.pause();
							}
							else {
								musStatus = "1";
								music.play();
							}
						}
						else if (i == 1) {//soundStatus
							if (soundStatus == "1") soundStatus = "0";
							else soundStatus = "1";
						}
						else if (i == 2) {//difficulty
							if (difficulty == "1") difficulty = "0";
							else difficulty = "1";
						}
					}
				}
			}

			if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				if (arrow1Sprite.getGlobalBounds().contains(pos.x, pos.y)) {//если мышь на стрелке
					int number = atoi(line.c_str());
					number--;
					if (number < 1) number = 5;
					line = std::to_string(number);
					hero.start(100, 190, line);
				}
				else if (arrow2Sprite.getGlobalBounds().contains(pos.x, pos.y)) {//если мышь на стрелке
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
			//0 up 1 down 2 left 3 right
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { //идём влево
				hero.updateFrame(1, (short)hero.CurrentFrame);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { //идём вправо
				hero.updateFrame(2, (short)hero.CurrentFrame);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { //идём вверх
				hero.updateFrame(3, (short)hero.CurrentFrame);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {//идём вниз
				hero.updateFrame(0, (short)hero.CurrentFrame);
			}

		}
		else {
			hero.CurrentFrame = 0;
			hero.updateFrame(0, 0);
		}

		if (selectedText != -1) {//если на что-то наведено
			setColor(color, ORANGE);
			if (pressedText == -1) {
				textM[selectedText].setFillColor(color);//покрасили текст
			}
			else {
				textM[selectedText].setFillColor(sf::Color::Black);//покрасили текст
				if (pressedText == 3) {//Если нажато выход
					status = false;
					position = 0;
					textM[selectedText].setFillColor(sf::Color::Black);
					std::ofstream out("settings.txt");
					out << "1.1" << std::endl;
					out << line << std::endl;
					out << musStatus << std::endl;
					out << soundStatus << std::endl;
					out << difficulty;
					out.close();
				}
			}

			textM[selectedText].setCharacterSize(54);//увеличели кегль
			textM[selectedText].setPosition(textM[selectedText].getPosition().x - 4, textM[selectedText].getPosition().y);//сдвигаем чуть-чуть влево чтобы текст увеличился из центра
		}

		window.draw(fon);//отрисовываем на экране фон
		window.draw(arrow1Sprite);
		window.draw(arrow2Sprite);
		window.draw(hero.heroSprite);
		for (int i = 0; i < 4; i++)
		{
			window.draw(textM[i]);
		}

		cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
		window.draw(cursor);
		window.setView(fixed);

		window.display();//обновить экран
	}
}

void printMenu(sf::RenderWindow &window, short &position, sf::Sprite &cursor) {//Отрисовка меню
	sf::Color color;
	setColor(color, POISONOUS_GREEN);
	//создание изображения

	sf::Texture fonTexture;
	sf::Sprite fon;
	createTexture(fonTexture, fon, "images/menuImage.jpg", 1024, 768);
	//fonTexture.~Texture();//удаляем текстуру так как она больше не нужна, но это только в теории
	//Текст и шрифт
	sf::Font font;//шрифт 
	font.loadFromFile("font.ttf");//передаем нашему шрифту файл шрифта
	sf::Text textM[4];
	for (int i = 0; i < 4; i++)
	{
		textM[i] = sf::Text("", font, 50);
		//textM[i].setStyle(sf::Text::Bold);
		textM[i].setFillColor(color);//покрасили текст. если убрать эту строку, то по умолчанию он белый

	}
	//по 256 на сектор
	textM[0].setString("Play");//задает строку тексту
	textM[0].setPosition(100, 30);//задаем позицию текста 
	textM[1].setString("Options");//задает строку тексту
	textM[1].setPosition(290, 30);//задаем позицию текста
	textM[2].setString("Author");//задает строку тексту
	textM[2].setPosition(560, 30);//задаем позицию текста
	textM[3].setString("Exit");//задает строку тексту
	textM[3].setPosition(820, 30);//задаем позицию текста


	bool status = true;

	short selectedText = -1;
	short pressedText = -1;

	sf::View fixed = window.getView(); // Create a fixed view

	while (status)
	{
		window.clear();//отчисщаем экран

		if (selectedText != -1) {//если какой-то текст был выделен
			setColor(color, POISONOUS_GREEN);
			textM[selectedText].setFillColor(color);//покрасили текст
			textM[selectedText].setCharacterSize(50);//вернули кегель
			textM[selectedText].setPosition(textM[selectedText].getPosition().x + 4, textM[selectedText].getPosition().y);//вернули начальную позицию

		}


		sf::Event event;
		while (window.pollEvent(event))
		{
			selectedText = -1;
			pressedText = -1;
			if (event.type == sf::Event::Closed) status = false;
			//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) status = false;
			//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Home)) ::SendNotifyMessage(window.getSystemHandle(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
			sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));//положение мыши
			for (int i = 0; i < 4; i++)
			{
				if (textM[i].getGlobalBounds().contains(pos.x, pos.y)) {//если мышь на тексте
					if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) pressedText = i;//если нажата левая кнопка выполняем

					selectedText = i;//выделяем текст
				}
			}
			//если нажато на портал
			if ((pow2(pos.x - 931) / pow2(70) + pow2(pos.y - 438) / pow2(305.5)) <= 1) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					status = false;
					position = -2;
				}
			}
		}

		if (selectedText != -1) {//если на что-то наведено
			setColor(color, ORANGE);
			if (pressedText == -1) {
				textM[selectedText].setFillColor(color);//покрасили текст
			}
			else {
				textM[selectedText].setFillColor(sf::Color::Black);//покрасили текст
				if (pressedText == 0) {//Если нажато новая игра
					position = 4;
				}
				else if (pressedText == 1) {//Если нажато настройки
					position = 2;
				}
				else if (pressedText == 2) {//Если нажато об авторе
					position = 1;
				}
				else if (pressedText == 3) {//Если нажато выход
					position = -1;
				}
				status = false;
			}

			textM[selectedText].setCharacterSize(54);//увеличели кегль
			textM[selectedText].setPosition(textM[selectedText].getPosition().x - 4, textM[selectedText].getPosition().y);//сдвигаем чуть-чуть влево чтобы текст увеличился из центра
		}

		window.draw(fon);//отрисовываем на экране тексты
		for (int i = 0; i < 4; i++)
		{
			window.draw(textM[i]);
		}

		cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
		window.draw(cursor);
		window.setView(fixed);

		window.display();//обновить экран
	}
}

void launch(sf::RenderWindow &window, short &position) {//Отрисовка окон
	sf::Texture fonTexture;
	sf::Sprite fon;
	createTexture(fonTexture, fon, "images/loadingImage.jpg", 1024, 768);
	
	sf::Texture cursorTexture;
	sf::Sprite cursor;
	createTexture(cursorTexture, cursor, "images/cursorImage.png", 32, 32);

	std::ifstream fin;
	std::string line = "1";
	fin.open("settings.txt");
	if (fin.is_open()) {//if the file is open
		getline(fin, line);// считали строку из файла
		getline(fin, line);// считали строку из файла
		getline(fin, line);// считали строку из файла
	}
	fin.close();

	sf::Music music;
	music.openFromFile("music/backgroundMusic.ogg");
	music.setLoop(true);
	if (line == "1") {
		music.play();
		music.setVolume(50);
	}

	while (position != -1) {
		if (position == 0)
		{
			printMenu(window, position, cursor);
		}
		else if (position == 1)
		{
			printAuthor(window, position, cursor);
		}
		else if (position == -2)
		{
			printError(window, position, cursor);
		}
		else if (position == 2)
		{
			printSettings(window, position, cursor, music);
		}
		else if (position == 3)
		{
			printEndGame(window, position, cursor);
		}
		else if (position > 3)
		{
			printGame(window, position, cursor, music);
		}
		if (position != -1) {
			window.clear();
			window.draw(fon);//отрисовываем на экране тексты
			window.display();//обновить экран
		}

	}
}
