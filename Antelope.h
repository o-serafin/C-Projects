#pragma once
#include "Animal.h"
#include <iostream>
#include <string>
using namespace std;

class Antelope : public Animal {
public:
	Antelope(int y, int x) {
		this->is_animal = true;
		this->alive = true;
		this->big_index = 2;
		this->initiative = 4;
		this->strength = 4;
		this->symbol = 'A';
		this->name = "Antelope";
		this->y = y;
		this->x = x;
		this->is_jumping = true;
	}
	void action(int sizeY, int sizeX, int move[8], Creature*** board) override;

	~Antelope() {

	}
};