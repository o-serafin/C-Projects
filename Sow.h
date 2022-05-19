#pragma once
#include "Creature.h"
#include "Plant.h"
#include <string>

using namespace std;

class Sow :public Plant {
public:
	Sow(int y, int x) {
		this->is_animal = false;
		this->alive = true;
		this->big_index = 6;
		this->initiative = 0;
		this->strength = 0;
		this->symbol = '%';
		this->name = "Sow Thistle";
		this->y = y;
		this->x = x;
	}

	void action(int sizeY, int sizeX, int move[8], Creature*** board) override;

	~Sow() {

	}
};
