#include <iostream>
#include <vector>
#include "ObserverPattern.h"

Subject::Subject() {};
Subject::~Subject() {};
void Subject::attach(Observer *obs) {
	views.push_back(obs);
}
void Subject::detach(Observer *obs) {
	for (int i = 0; i < views.size(); i++) {
		if (views[i] == obs) {
			views.erase(views.begin() + i);
			break;
		}
	}
}
void Subject::notify() {
	for (int i = 0; i < views.size(); i++)
		views[i]->update(this);
}

