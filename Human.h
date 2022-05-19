#pragma once
#include "Creature.h"
#include "Animal.h"
#include <iostream>
#include <string>
using namespace std;


class Human : public Animal {
	
public:
	
	Human(int y, int x) {
		this->is_animal = true;
		this->alive = true;
		this->initiative = 4;
		this->strength = 5;
		this->symbol = 'H';
		this->name = "Human";
		this->y = y;
		this->x = x;
	}
	void action(int sizeY, int sizeX, int move[8], Creature*** board) override;

	void weaken() override; 

	~Human() {

	}
};