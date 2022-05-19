#include "Antelope.h"

void Antelope::action(int sizeY, int sizeX, int move[8], Creature*** board) {
	int xi = this->x;
	int yi = this->y;
	for (int z = 0;z < 2;z++) {
		int dir = rand() % 2; // 0 for horizontal 1 for vertical
		int val = rand() % 2;
		if (val == 0) {
			val = -1;
		}
		if (dir == 1) {
			if (yi + val < 0 || yi + val >= sizeY) {
				move[0] -= val;
				yi -= val;
			}
			else {
				move[0] += val;
				yi += val;
			}
		}
		else {
			if (xi + val < 0 || xi + val >= sizeX) {
				move[1] -= val;
				xi -= val;
			}
			else {
				move[1] += val;
				xi += val;
			}
		}
	}
}