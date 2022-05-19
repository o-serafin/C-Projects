#include "Hogweed.h"

void Hogweed::action(int sizeY, int sizeX, int move[8], Creature*** board) {
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
	for (int i = -1;i < 2;i++) {
		for (int j = -1;j < 2;j++) {
			if (this->y + i >= 0 && this->y + i < sizeY && this->x + j >= 0 && this->x + j < sizeX && abs(i) != abs(j)) {
				if (board[this->y + i][this->x + j] != NULL && board[this->y + i][this->x + j]->is_animal == true) {
					move[yi] = this->y + i;
					move[xi] = this->x + j;
					yi += 2;
					xi += 2;
				}
			}
		}
	}
}