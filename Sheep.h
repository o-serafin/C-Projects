#pragma once

#include "Animal.h"
#include <iostream>
#include <string>
using namespace std;


class Sheep : public Animal {
public:


	Sheep(int y, int x) {
		this->is_animal = true;
		this->alive = true;
		this->big_index = 2;
		this->initiative = 4;
		this->strength = 4;
		this->symbol = 'S';
		this->name = "Sheep";
		this->y = y;
		this->x = x;
	}

	~Sheep() {

	}
};

