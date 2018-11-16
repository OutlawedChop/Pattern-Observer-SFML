#ifndef OBSERVER_PATTERN
#define OBSERVER_PATTERN
#pragma once
#include <iostream>
#include <vector>

using namespace std;
class Subject {
private:
	vector < class Observer * > views;
protected:
	Subject();
public:
	virtual ~Subject();
	void attach(Observer *obs);
	void detach(Observer *obs);
	void notify();
};

class Observer {
public:
	Observer() {};
	virtual void update(Subject *model) = 0;
};

#endif