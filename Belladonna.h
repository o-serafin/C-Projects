#pragma once

#include "Creature.h"
#include "Plant.h"
#include <string>

using namespace std;

class Belladonna :public Plant {
public:
	Belladonna(int y, int x) {
		this->is_animal = false;
		this->alive = true;
		this->big_index = 8;
		this->initiative = 0;
		this->strength = 99;
		this->symbol = '+';
		this->name = "Belladonna";
		this->y = y;
		this->x = x;
	}
	~Belladonna() {

	}
};
