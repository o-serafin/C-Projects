#pragma once
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>

using namespace std;

class Creature {

protected:
	
	int age;
	int x, y;
	int strength;
	int initiative;
	int big_index;
	
public:
	bool is_animal;
	int turns_remaining = 0;
	bool has_shell = false;
	bool is_jumping = false;
	bool alive;
	string name;
	char symbol;

	virtual void increse_strength(int n) {
		this->strength += n;
	}

	virtual int getX() {
		return x;
	}

	virtual int getY() {
		return y;
	}

	virtual int get_strength() {
		return strength;
	}

	virtual void change_coords(int y, int x) {
		this->x = x;
		this->y = y;
	}

	virtual void weaken() {};

	virtual void action(int sizeY, int sizeX, int move[8], Creature*** board) = 0;

	virtual void collision_multiply(Creature*** board, int y_new, int x_new, int sizeX, int sizeY, int pos_of_child[2]) = 0;
	
	virtual int no_fight(Creature*** board, int y_new, int x_new, int new_pos[2], int sizeX, int sizeY) = 0;

};
