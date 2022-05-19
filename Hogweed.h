#pragma once
#include "Creature.h"
#include "Plant.h"
#include <string>

using namespace std;

class Hogweed :public Plant {
public:
	Hogweed(int y, int x) {
		this->is_animal = false;
		this->alive = true;
		this->big_index = 5;
		this->initiative = 0;
		this->strength = 10;
		this->symbol = '#';
		this->name = "Hogweed";
		this->y = y;
		this->x = x;
	}

	void action(int sizeY, int sizeX, int move[8], Creature*** board) override;

	~Hogweed() {

	}
};
