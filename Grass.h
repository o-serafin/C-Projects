#pragma once
#include "Creature.h"
#include "Plant.h"
#include <string>

using namespace std; 

class Grass :public Plant {
public:
	Grass(int y, int x) {
		this->is_animal = false;
		this->alive = true;
		this->big_index = 4;
		this->initiative = 0;
		this->strength = 0;
		this->symbol = ',';
		this->name = "Grass";
		this->y = y;
		this->x = x;
	}

	~Grass() {

	}
};