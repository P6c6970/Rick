//#include <Windows.h>//для сворачивания окна
//#include <iostream>
#include "def.h"

int main()
{
	//создание окна
	short position = 0;
	//sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Rick", sf::Style::Fullscreen);

	sf::RenderWindow window(sf::VideoMode(1024,768), "Rick", sf::Style::Fullscreen);
	window.setMouseCursorVisible(false);//скрыть курсор
	window.display();//чтобы оно не было белым
	launch(window, position);

	window.close();
}

