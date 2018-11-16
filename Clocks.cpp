#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <sstream>
#include <string>
#include "ObserverPattern.h"
#include "Clocks.h"

//Timer class
ClockTimer::ClockTimer() {};
ClockTimer::ClockTimer(int hour, int minute, int second) {
	this->hour = hour;
	this->minute = minute;
	this->second = second;
}
int ClockTimer::GetHour() {
	return hour;
}
int ClockTimer::GetMinute() {
	return minute;
}
int ClockTimer::GetSecond() {
	return second;
}
void ClockTimer::Tick(int hour, int minute, int second) {
	this->hour = hour;
	this->minute = minute;
	this->second = second;
	notify();
};
void ClockTimer::Tick() {
	notify();
}


//Segment Clock helper class
SevenSegment::SevenSegment() : SevenSegment(0, 0, 20, 100) {};
SevenSegment::~SevenSegment() {
	delete[] shapes;
}
SevenSegment::SevenSegment(float x, float y, float width, float height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	shapes = new RectangleShape[7];
	for (int i = 0; i < SEGMENTS_AMOUNT; i++) {
		if (i != 0 && i != 3 && i != 6) {
			shapes[i] = RectangleShape(Vector2f(width, height));
		}
		else {
			shapes[i] = RectangleShape(Vector2f(height, width));
		}
		shapes[i].setFillColor(Color::Red);
	}

	this->x = x;
	this->y = y;
	shapes[6].setPosition(x + width, y - width);
	shapes[0].setPosition(x + width, y + height);
	shapes[3].setPosition(x + width, y + 2 * height + width);

	shapes[1].setPosition(x, y);
	shapes[5].setPosition(x + height + width, y);
	shapes[2].setPosition(x, y + height + width);
	shapes[4].setPosition(x + height + width, y + height + width);

}
void SevenSegment::update(int value) {
	for (int i = 0; i < SEGMENTS_AMOUNT; i++) {
		if (((nums[value] >> i) & 1)) {
			shapes[i].setFillColor(Color::Red);
		}
		else {
			shapes[i].setFillColor(Color::Black);
		}
	}
}
void  SevenSegment::draw(sf::RenderWindow &window) {
	for (int i = 0; i < SEGMENTS_AMOUNT; i++) {
		window.draw(shapes[i]);
	}
}


//Seven Segment Clock class
SevenSegmentsClock::SevenSegmentsClock(ClockTimer *timer, float x, float y, float width, float height) {
	_subject = timer;
	_subject->attach(this);
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	empty_space = height;
	int add = 0;
	circles = new CircleShape[AMOUNT_OF_CIRCLES];
	time_numbers = new SevenSegment*[AMOUNT_OF_TIME_NUMBERS];
	for (int i = 0; i < AMOUNT_OF_TIME_NUMBERS; i++) {
		time_numbers[i] = new SevenSegment(x + (2.5 * width + height) * i + ((i - i % 2) / 2) * empty_space, y, width, height);
	}
	for (int i = 0; i < AMOUNT_OF_CIRCLES; i++) {
		if (i >= 2) {
			add = (5 * width + 2 * height + empty_space);
		}
		circles[i] = CircleShape(width);
		circles[i].setFillColor(Color::Red);
		circles[i].setPosition(x + (4 * width + 2 * height + empty_space / 2) + add,
			y + height / 2 - width + (height + width) * (i % 2));
	}
}
SevenSegmentsClock::~SevenSegmentsClock() {
	_subject->detach(this);
	for (int i = 0; i < AMOUNT_OF_TIME_NUMBERS; i++) {
		delete time_numbers[i];
	}
	delete time_numbers;
	delete[] circles;
}
void SevenSegmentsClock::update(Subject *theChangedSubject) {
	if (theChangedSubject = _subject) {
		time_numbers[1]->update(_subject->GetHour() % 10);
		time_numbers[3]->update(_subject->GetMinute() % 10);
		time_numbers[5]->update(_subject->GetSecond() % 10);

		if (_subject->GetHour() > 9) {
			time_numbers[0]->update((int)_subject->GetHour() / 10);
		}
		else {
			time_numbers[0]->update(0);
		}
		if (_subject->GetMinute() > 9) {
			time_numbers[2]->update((int)_subject->GetMinute() / 10);
		}
		else {
			time_numbers[2]->update(0);
		}
		if (_subject->GetSecond() > 9) {
			time_numbers[4]->update((int)_subject->GetSecond() / 10);
		}
		else {
			time_numbers[4]->update(0);
		}
	}
	//time_numbers
}
void SevenSegmentsClock::draw(RenderWindow &window) {
	for (int i = 0; i < AMOUNT_OF_TIME_NUMBERS; i++) {
		time_numbers[i]->draw(window);
	}
	for (int i = 0; i < AMOUNT_OF_CIRCLES; i++) {
		window.draw(circles[i]);
	}
}


//Analog Clock class
AnalogClock::AnalogClock(ClockTimer *timer, float x, float y, float scale) {
	this->_subject = timer;
	_subject->attach(this);
	this->x = x;
	this->y = y;
	this->scale = scale;

	sound = new Sound;

	soundBuffer = new SoundBuffer;
	soundBuffer->loadFromFile("resources/tick.ogg");
	sound->setBuffer(*soundBuffer);

	hands = new RectangleShape[AMOUNT_OF_HANDS];
	texture = new Texture;
	image = new Image;

	image->loadFromFile("resources/clock_face.jpg");
	image->createMaskFromColor(Color(0, 0, 0));;
	texture->loadFromImage(*image);
	
	radius = texture->getSize().x * scale / 2;

	clock.setPosition(Vector2f(x - radius, y - radius));
	clock.setTexture(*texture);
	clock.scale(scale, scale);

	centre = CircleShape(radius / 30);
	centre.setOrigin(Vector2f(radius / 60, radius / 60));
	centre.setPosition(Vector2f(x - radius / 60, y - radius / 60));
	centre.setFillColor(Color::Black);

	
	for (int i = 0; i < AMOUNT_OF_HANDS; i++) {
		hands[i] = RectangleShape(Vector2f(radius * (0.8 - i * 0.1), 2 + i));
		hands[i].setOrigin(Vector2f(hands[i].getSize().x - 0.1 * radius, hands[i].getSize().y / 2));
		hands[i].setPosition(Vector2f(x, y));
		hands[i].setFillColor(Color::Black);
	}
}
AnalogClock::~AnalogClock() {
	_subject->detach(this);
	delete[] hands;
	delete texture;
	delete image;
	delete soundBuffer;
	delete sound;
}
void AnalogClock::draw(RenderWindow &window) {
	window.draw(clock);
	for (int i = 0; i < AMOUNT_OF_HANDS; i++) {
		window.draw(hands[i]);
	}
	window.draw(centre);
}
void AnalogClock::update(Subject *theChangedSubject) {

	if (sound->getStatus() == 0) {
		sound->play();
	}
	if (_subject == theChangedSubject) {
		hands[0].setRotation(_subject->GetSecond()	* 6		-	270);
		hands[1].setRotation(_subject->GetMinute()	* 6		-	270);
		hands[2].setRotation(_subject->GetHour()	* 30	-	270);
	}
};


//Digital Clock class
DigitalClock::DigitalClock(ClockTimer *timer, float x, float y, float size) {
	_subject = timer;
	_subject->attach(this);
	
	font = new Font;
	font->loadFromFile("resources/font.ttf");
	text.setFont(*font);
	text.setPosition(x, y);
	text.setCharacterSize(size);
	text.setFillColor(Color::Cyan);
	text.setString("");
}
DigitalClock::~DigitalClock() {
	_subject->detach(this);
	delete font;
}
void DigitalClock::draw(RenderWindow &window) {
	window.draw(text);
}
void DigitalClock::update(Subject *theChangedSubject) {
	if (theChangedSubject == _subject) {
		string temp;
		string cur_string;
		stringstream ss;
		if (_subject->GetHour() < 10) {
			cur_string += "0";
		}
		ss << _subject->GetHour();
		ss >> temp;
		ss.clear();
		cur_string = cur_string + temp + ". ";

		if (_subject->GetMinute() < 10) {
			cur_string += "0";
		}
		ss << _subject->GetMinute();
		ss >> temp;
		ss.clear();
		cur_string = cur_string + temp + ". ";

		if (_subject->GetSecond() < 10) {
			cur_string += "0";
		}
		ss << _subject->GetSecond();
		ss >> temp;
		ss.clear();
		cur_string = cur_string + temp;
		
		text.setString(cur_string);
	}
}