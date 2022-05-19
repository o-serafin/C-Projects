#include "Turtle.h"
#include <cstdlib>
#include <time.h>

void Turtle::action(int sizeY, int sizeX, int move[8], Creature*** board) {
	int chance = rand() % 4;
	if (chance != 2) {
		return;
	}
	Animal::action(sizeY, sizeX, move, board);
}