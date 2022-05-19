#pragma once
#include "Creature.h"
#include <cstdlib>
#include <time.h>

class Plant : public Creature {

	virtual void collision_multiply(Creature*** board, int y_new, int x_new, int sizeX, int sizeY, int pos_of_child[2]) {
		
	}

	int no_fight(Creature*** board, int y_new, int x_new, int new_pos[2], int sizeX, int sizeY) {
		return 0;
	}

	virtual void action(int sizeY, int sizeX, int move[8], Creature*** board) {
		for (int i = 0; i < 8; i++) {
			if (i % 2 == 0) {
				move[i] = this->y;
			}
			else {
				move[i] = this->x;
			}
		}
		int sow = rand() % 30;
		int yi = 0;
		int xi = 1;
		if (sow == 1) {
			int i = (rand() % 2) - 1;
			int j = (rand() % 2) - 1;
			int count = 0;
			while (count < 100) {
				count++;
				if (this->y + i >= 0 && this->y + i < sizeY && this->x + j >= 0 && this->x + j < sizeX && board[this->y + i][this->x + j] == NULL) {
					move[yi] = this->y + i;
					move[xi] = this->x + j;
					break;
				}
				else{
					i = (rand() % 2) - 1;
					j = (rand() % 2) - 1;
				}
			}
		}
	}
};

