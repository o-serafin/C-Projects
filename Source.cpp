#include <iostream>
#include <string>
#include "World.h"
#include <conio.h>
#include <windows.h>
#include <fstream>

using namespace std;

int main() {
	srand(time(NULL));
	World world = World();
	world.create_map(15, 15);
	world.create_world();
	world.print_world();
	while (true) {
		world.world_cycle();
	}
	return 0;
}
