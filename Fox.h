#pragma once
#include "Animal.h"

#include <iostream>
#include <string>
using namespace std;


class Fox : public Animal {
public:

	Fox(int y, int x) {
		this->is_animal = true;
		this->alive = true;
		this->big_index = 0;
		this->initiative = 7;
		this->strength = 3;
		this->symbol = 'F';
		this->name = "Fox";
		this->y = y;
		this->x = x;
	}
	void action(int sizeY, int sizeX, int move[8], Creature*** board) override;

	~Fox() {

	}
};

