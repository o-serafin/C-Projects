#pragma once

#include "Creature.h"
#include "Plant.h"
#include <string>

using namespace std;

class Guarana :public Plant {
public:
	Guarana(int y, int x) {
		this->is_animal = false;
		this->alive = true;
		this->big_index = 7;
		this->initiative = 0;
		this->strength = 0;
		this->symbol = 'G';
		this->name = "Guarana";
		this->y = y;
		this->x = x;
	}

	~Guarana() {

	}
};
