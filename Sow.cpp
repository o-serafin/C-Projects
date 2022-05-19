#include "Sow.h"
#include <cstdlib>
#include <time.h>


void Sow::action(int sizeY, int sizeX, int move[8], Creature*** board) {
	for (int i = 0; i < 8; i++) {
		if (i % 2 == 0) {
			move[i] = this->y;
		}
		else {
			move[i] = this->x;
		}
	}
	int yi = 0;
	int xi = 1;
	for (int z = 0; z < 3; z++) {
		int sow = rand() % 30;
		if (sow == 1) {
			int i = (rand() % 2) - 1;
			int j = (rand() % 2) - 1;
			int count = 0;
			while (count<100) {
				count++;
				if (this->y + i >= 0 && this->y + i < sizeY && this->x + j >= 0 && this->x + j < sizeX && board[this->y + i][this->x + j] == NULL) {
					move[yi] = this->y + i;
					move[xi] = this->x + j;
					break;
				}
				else {
					i = (rand() % 2) - 1;
					j = (rand() % 2) - 1;
				}
			}
		}
	}
}

