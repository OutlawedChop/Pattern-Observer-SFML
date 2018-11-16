#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "windows.h"
#include "Clocks.h"
#include "ObserverPattern.h"
using namespace std;



int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1200, 800), "Clocks", sf::Style::Default, settings);

	SYSTEMTIME st;
	GetLocalTime(&st);


	ClockTimer timer(st.wHour,st.wMinute,st.wSecond);
	SevenSegmentsClock	*s_clock = new SevenSegmentsClock	(&timer, 100, 150, 13, 50);
	AnalogClock			*a_clock = new AnalogClock			(&timer, 950, 375, 0.5);
	DigitalClock		*d_clock = new DigitalClock			(&timer, 150, 450, 165);


	Sprite sprite;
	Texture *texture = new Texture;
	texture->loadFromFile("resources/time.jpg");
	sprite.setTexture(*texture);

	window.setVerticalSyncEnabled(true);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		
		//////////////////////
		GetLocalTime(&st);
		timer.Tick(st.wHour, st.wMinute, st.wSecond);
		//timer.detach(s_clock);
		//timer.detach(a_clock);
		//////////////////////

		window.draw(sprite);
		s_clock->draw(window);
		a_clock->draw(window);
		d_clock->draw(window);
		window.display();
	}
	return 0;
}