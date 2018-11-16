#ifndef SEVEN_SEGMENTS_CLOCK
#define SEVEN_SEGMENTS_CLOCK
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <windows.h>
#include "ObserverPattern.h"
#define EMPTY_SPACE				60
#define SEGMENTS_AMOUNT			7
#define AMOUNT_OF_TIME_NUMBERS	6
#define AMOUNT_OF_CIRCLES		4
#define AMOUNT_OF_HANDS			3

using namespace std;
using namespace sf;

const int nums[10] = { 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B };

//Timer class
class ClockTimer : public Subject{
private:
	int hour;
	int minute;
	int second;
public:
	ClockTimer();
	ClockTimer(int hour, int minute, int second);
	virtual int GetHour();
	virtual int GetMinute();
	virtual int GetSecond();
	void Tick(int hour, int minute, int second);
	void Tick();
};

//Clock Shape
class ClockShape {
protected:
	float x;
	float y;
public:
	ClockShape() : x(0), y(0), _subject(&ClockTimer()) {};
	ClockTimer *_subject;
	virtual void draw(RenderWindow &window) = 0;
};


//Segment Clock helper class
class SevenSegment {
public:
	float x;
	float y;
	float width;
	float height;
	RectangleShape *shapes;
public:
	SevenSegment();
	~SevenSegment();
	SevenSegment(float x, float y, float width, float height);
	void update(int value);
	void draw(sf::RenderWindow &window);
};
//Segment Clock class
class SevenSegmentsClock : public ClockShape, public Observer{
private:
	SevenSegment **time_numbers;
	float height;
	float width;
	float empty_space;
	CircleShape *circles;
public:
	//width and height of texture
	SevenSegmentsClock(ClockTimer *timer, float x, float y, float width, float height);
	~SevenSegmentsClock();
	void draw(RenderWindow &window);
	void update(Subject *model);
};


//Analog Clock class
class AnalogClock : public ClockShape, public Observer{
private:
	float radius;
	float scale;
	RectangleShape *hands;
	Texture *texture;
	Image *image;
	CircleShape centre;
	Sprite clock;
	SoundBuffer *soundBuffer;
	Sound *sound;
public:
	AnalogClock(ClockTimer *timer, float x, float y, float scale);
	~AnalogClock();
	void draw(RenderWindow &window);
	void update(Subject *theChangedSubject);
};


//Digital Clock class
class DigitalClock : public ClockShape, public Observer {
private:
	float size;
	Text text;
	Font *font;
public:
	DigitalClock(ClockTimer *timer, float x, float y, float size);
	~DigitalClock();
	void draw(RenderWindow &window);
	virtual void update(Subject *theChangedSubject);
};
#endif