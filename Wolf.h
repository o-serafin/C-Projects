#pragma once

#include "Animal.h"
#include <iostream>
#include <string>
using namespace std;

class Wolf : public Animal {
public:
	
	Wolf(int y, int x) {
		this->is_animal = true;
		this->alive = true;
		this->big_index = 1;
		this->initiative = 5;
		this->strength = 9;
		this->symbol = 'W';
		this->name = "Wolf";
		this->y = y;
		this->x = x;
	}
	~Wolf() {

	}
};
