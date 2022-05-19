#pragma once

#include "Animal.h"
#include <iostream>
#include <string>
using namespace std;

class Turtle : public Animal {
public:

	Turtle(int y, int x) {
		this->has_shell = true;
		this->is_animal = true;
		this->alive = true;
		this->big_index = 3;
		this->initiative = 1;
		this->strength = 2;
		this->symbol = 'T';
		this->name = "Turtle";
		this->y = y;
		this->x = x;
	}
	void action(int sizeY, int sizeX, int move[8], Creature*** board) override;

	~Turtle() {

	}
};
