#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <conio.h>
#include <vector>
#include <fstream>

#include "Wolf.h"
#include "Turtle.h"
#include "Fox.h"
#include "Antelope.h"
#include "Sheep.h"
#include "Human.h"
#include "Grass.h"
#include "Sow.h"
#include "Hogweed.h"
#include "Guarana.h"
#include "Belladonna.h"

class Creature;

using namespace std;

class World {
public:
	
	int sizeX, sizeY;
	int number_of_animals[9] = {0,0,0,0,0,0,0,0,0};
	string names[10] = { "Fox","Wolf","Sheep","Antelope","Turtle","Grass","Hogweed","Sow Thistle","Guarana", "Belladonna" };
	Creature*** board;
	Creature* animals[9][200];
	Creature* human;
	vector<string> messages;


	World() {
		
	}

	void add_human(int y, int x) {
		human = new Human(y, x);
		board[y][x] = human;
	}
	
	void add_Fox(int y, int x) {
		animals[0][number_of_animals[0]] = new Fox(y, x);
		board[y][x] = animals[0][number_of_animals[0]];
		number_of_animals[0] += 1;
	}

	void add_Wolf(int y, int x) {
		animals[1][number_of_animals[1]] = new Wolf(y, x);
		board[y][x] = animals[1][number_of_animals[1]];
		number_of_animals[1] += 1;
	}

	void add_Sheep(int y, int x) {
		animals[2][number_of_animals[2]] = new Sheep(y, x);
		board[y][x] = animals[2][number_of_animals[2]];
		number_of_animals[2] += 1;
	}

	void add_Antelope(int y, int x) {
		animals[2][number_of_animals[2]] = new Antelope(y, x);
		board[y][x] = animals[2][number_of_animals[2]];
		number_of_animals[2] += 1;
	}

	void add_Turtle(int y, int x) {
		animals[3][number_of_animals[3]] = new Turtle(y, x);
		board[y][x] = animals[3][number_of_animals[3]];
		number_of_animals[3] += 1;
	}
	
	void add_Grass(int y, int x) {
		animals[4][number_of_animals[4]] = new Grass(y, x);
		board[y][x] = animals[4][number_of_animals[4]];
		number_of_animals[4] += 1;
	}

	void add_Hogweed(int y, int x) {
		animals[5][number_of_animals[5]] = new Hogweed(y, x);
		board[y][x] = animals[5][number_of_animals[5]];
		number_of_animals[5] += 1;
	}

	void add_Sow(int y, int x) {
		animals[6][number_of_animals[6]] = new Sow(y, x);
		board[y][x] = animals[6][number_of_animals[6]];
		number_of_animals[6] += 1;
	}

	void add_Guarana(int y, int x) {
		animals[7][number_of_animals[7]] = new Guarana(y, x);
		board[y][x] = animals[7][number_of_animals[7]];
		number_of_animals[7] += 1;
	}

	void add_Belladonna(int y, int x) {
		animals[8][number_of_animals[8]] = new Belladonna(y, x);
		board[y][x] = animals[8][number_of_animals[8]];
		number_of_animals[8] += 1;
	}

	void create_map(int size_Y, int size_X){
		this->sizeX = size_X;
		this->sizeY = size_Y;
		board = new Creature * *[sizeX];
		for (int i = 0; i < sizeY; ++i) {
			board[i] = new Creature * [sizeX];
		}
		for (int i = 0; i < sizeY; ++i) {
			for (int j = 0;j < sizeX;j++) {
				board[i][j] = NULL;
			}
		}
	}

	void create_world() {
		int beg[10];
		int y_h = rand() % sizeY;
		int x_h = rand() % sizeX;
		add_human(y_h, x_h);
		for (int i = 0;i < 10;i++) {
			cout << "Enter number of " << names[i] << ": ";
			cin >> beg[i];
			for (int j = 0;j < beg[i];j++) {
				int y = rand() % sizeY;
				int x = rand() % sizeX;
				while (board[y][x] != NULL) {
					y = rand() % sizeY;
					x = rand() % sizeX;
				}
				if (i == 0) {
					add_Fox(y, x);
				}
				else if (i == 1) {
					add_Wolf(y, x);
				}
				else if (i == 2) {
					add_Sheep(y, x);
				}
				else if (i == 3) {
					add_Antelope(y, x);
				}
				else if (i == 4) {
					add_Turtle(y, x);
				}
				else if (i == 5) {
					add_Grass(y, x);
				}
				else if (i == 6) {
					add_Hogweed(y, x);
				}
				else if (i == 7) {
					add_Sow(y, x);
				}
				else if (i == 8) {
					add_Guarana(y, x);
				}
				else if (i == 9) {
					add_Belladonna(y, x);
				}
			}
		}
		system("cls");
	}
	
	void move_plants(Creature &creature, int move[8]) {
		if (creature.symbol == '#') {
			for (int i = 0; i < 4; i++) {
				int j = i * 2;
				if (!(move[j] == creature.getY() && move[j + 1] == creature.getX())) {
					Creature* victim = board[move[j]][move[j+1]];
					board[move[j]][move[j+1]] = NULL;
					victim->alive = false;
					messages.push_back(victim->name + " is killed by Hogweed");
				}
			}
		}
		else {
			for (int i = 0; i < 7; i += 2) {
				if (!(move[i] == creature.getY() && move[i + 1] == creature.getX())) {
					
					if (creature.name == "Guarana" && number_of_animals[7] < 200) {
						add_Guarana(move[i],move[i+1]);
					}
					else if (creature.name == "Belladonna" && number_of_animals[8] < 200) {
						add_Belladonna(move[i], move[i + 1]);
					}
					else if (creature.name == "Sow Thistle" && number_of_animals[6] < 200) {
						add_Sow(move[i], move[i + 1]);
					}
					else if (creature.name == "Grass" && number_of_animals[4] < 200) {
						add_Grass(move[i], move[i + 1]);
					}
					messages.push_back("New " + creature.name + " is born on y=" + to_string(move[i]) + " x=" + to_string(move[i + 1]) + " from y = " + to_string(creature.getY()) + " x = " + to_string(creature.getX()));
				}
			}
		}
	}

	void move_animal(Creature& creature, int move[8]) {
		if (board[creature.getY() + move[0]][creature.getX() + move[1]] == NULL) {
			board[creature.getY()][creature.getX()] = NULL;
			board[creature.getY() + move[0]][creature.getX() + move[1]] = &creature;
			creature.change_coords(creature.getY() + move[0], creature.getX() + move[1]);
		}
		else if (move[0] == 0 && move[1] == 0) {

		}
		else if (board[creature.getY() + move[0]][creature.getX() + move[1]]->symbol == creature.symbol) {
			int child_pos[2] = { creature.getY() + move[0],creature.getX() + move[1] };
			creature.collision_multiply(board, creature.getY() + move[0], creature.getX() + move[1], sizeX, sizeY, child_pos);
			if (child_pos[0] == creature.getY() + move[0] && child_pos[1] == creature.getX() + move[1]) {
				messages.push_back("Unable to find place for child");
			}
			else {
				if (creature.name == "Turtle" && number_of_animals[3] < 200) {
					add_Turtle(child_pos[0], child_pos[1]);
				}
				else if (creature.name == "Fox" && number_of_animals[0] < 200) {
					add_Fox(child_pos[0], child_pos[1]);
				}
				else if (creature.name == "Sheep" && number_of_animals[2] < 200) {
					add_Sheep(child_pos[0], child_pos[1]);
				}
				else if (creature.name == "Antelope" && number_of_animals[2] < 200) {
					add_Antelope(child_pos[0], child_pos[1]);
				}
				else if (creature.name == "Wolf" && number_of_animals[1] < 200) {
					add_Wolf(child_pos[0], child_pos[1]);
				}
				messages.push_back("New " + creature.name + " is born on y=" + to_string(child_pos[0]) + " x=" + to_string(child_pos[1]));
			}
		}
		else {
			int new_pos[2] = { 0,0 };
			int no_fight = creature.no_fight(board, creature.getY() + move[0], creature.getX() + move[1], new_pos, sizeX, sizeY);
			if (no_fight == 1) {
				return;
			}
			else if (no_fight == 2) {
				Creature* antelope = board[creature.getY() + move[0]][creature.getX() + move[1]];
				board[new_pos[0]][new_pos[1]] = antelope;
				antelope->change_coords(new_pos[0], new_pos[1]);
				board[creature.getY() + move[0]][creature.getX() + move[1]] = &creature;
				board[creature.getY()][creature.getX()] = NULL;
				creature.change_coords(creature.getY() + move[0], creature.getX() + move[1]);
				messages.push_back("Antelope escaped to field y=" + to_string(antelope->getY()) + " x=" + to_string(antelope->getX()));
			}
			else {
				Creature* victim = board[creature.getY() + move[0]][creature.getX() + move[1]];
				if (creature.get_strength() < victim->get_strength() || victim->symbol == '#') {
					creature.alive = false;
					board[creature.getY()][creature.getX()] = NULL;
					messages.push_back(creature.name + " is killed by " + victim->name);
				}
				else {
					victim->alive = false;
					if (victim->name == "Guarana") {
						creature.increse_strength(3);
					}
					board[victim->getY()][victim->getX()] = &creature;
					board[creature.getY()][creature.getX()] = NULL;
					creature.change_coords(creature.getY() + move[0], creature.getX() + move[1]);
					messages.push_back(victim->name + " is killed by " + creature.name);
				}
			}
		}
	}

	void save_world() {
		ofstream myfile;
		myfile.open("game_save.txt");
		myfile << sizeY <<" " << sizeX << '\n';
		if (human->alive) {
			myfile << "H " << human->getY() << " " << human->getX() << '\n';
		}
		for (int y = 0; y < 9; y++) {
			for (int x = 0; x < number_of_animals[y];x++) {
				if (animals[y][x]->alive) {
					myfile << animals[y][x]->symbol << " " << animals[y][x]->getY() << " " << animals[y][x]->getX() << " " << '\n';
				}		
			}
		}
		myfile.close();
	}

	void delete_world() {
		delete human ;
		for (int i = 0;i < 9;i++) {
			number_of_animals[i] = 0;
		}
		for (int i = 0; i < 9;i++) {
			for (int j = 0;j < 200;j++) {
				if (animals[i][j] != NULL) {
					delete animals[i][j];
					animals[i][j] = NULL;
				}
			}
		}
		for (int i = 0; i < sizeY;i++) {
			delete[] board[i];
		}
		delete[] board;
		messages.clear();
	}

	void add_from_file(string symbol, int y, int x) {
		if (symbol=="F") {		
			add_Fox(y, x);
		}
		else if (symbol == "W") {
			add_Wolf(y, x);
		}
		else if (symbol == "S") {
			add_Sheep(y, x);
		}
		else if (symbol == "A") {
			add_Antelope(y, x);
		}
		else if (symbol == "T") {
			add_Turtle(y, x);
		}
		else if (symbol == ",") {
			add_Grass(y, x);
		}
		else if (symbol == "#") {
			add_Hogweed(y, x);
		}
		else if (symbol == "%") {
			add_Sow(y, x);
		}
		else if (symbol == "G") {
			add_Guarana(y, x);
		}
		else if (symbol == "+") {
			add_Belladonna(y, x);
		}
		else if (symbol == "H") {
			add_human(y, x);
		}
	}

	void load_world() {
		delete_world();
		string line;
		ifstream myfile("game_save.txt");
		int iter = 0;
		string symbol;
		int cords[2];
		while (myfile.good())
		{	
			string line;
			myfile >> line;
			if (myfile.eof()) {
				break;
			}
			if (iter == 0) {
				cords[0] = stoi(line);
			}
			else if (iter == 1) {
				cords[1] = stoi(line);
				create_map(cords[0], cords[1]);
			}
			else if ((iter+1)%3 == 0) {
				symbol = line;
			}
			else if ((iter) % 3 == 0) {
				cords[0] = stoi(line);
			}
			else if ((iter-1) % 3 == 0) {
				cords[1] = stoi(line);
				add_from_file(symbol, cords[0], cords[1]);
			}
			iter++;
		}
		myfile.close();
	}

	void print_world() {
		for (int i = 0; i < sizeY+2; ++i) {
			for (int j = 0;j < sizeX+2;j++) {
				if (i == 0 || i == sizeY + 1) {
					cout << '-';
				}
				else if (j == 0 || j == sizeX + 1 && i != 0 && i != sizeY + 1) {
					cout << '|';
				}
				else {
					if (board[i-1][j-1] == NULL) {
						cout << ' ';
					}
					else {
						cout << this->board[i-1][j-1]->symbol;
					}
				}			
			}
			cout << endl;
		}
		cout << endl;
	}

	void world_cycle() {
		int human_move[8] = { 0,0 };
		if (human->alive) {
			human->action(sizeY, sizeX, human_move, board);
			if (human_move[0] == 8888) {
				save_world();
				human_move[0] = 0;
				human_move[1] = 0;
			}
			else if (human_move[0] == 9999) {
				load_world();
				human_move[0] = 0;
				human_move[1] = 0;
			}
			
		}
		else {
			char a =_getwch();
			if (a == 's') {
				save_world();
			}
			else if (a == 'l') {
				load_world();
			}
		}
		system("cls");
		for (int i = 0;i < 9;i++) {
			if (i == 2 && human->alive) {
				move_animal(*human, human_move);
				human->weaken();
			}
			if (i < 4) {
				for (int j = 0;j < number_of_animals[i];j++) {
					if (animals[i][j]->alive) {
						int move[8] = { 0,0 };
						animals[i][j]->action(sizeY, sizeX, move, board);
						move_animal(*animals[i][j], move);
					}
				}
			}
			else {
				for (int j = 0;j < number_of_animals[i];j++) {
					if (animals[i][j]->alive) {
						int move[8];
						animals[i][j]->action(sizeY, sizeX, move, board);
						move_plants(*animals[i][j], move);
					}
				}
			}
		}
		print_world();
		for (int i = 0; i < messages.size(); i++) {
			cout << messages[i] << endl;
		}
		messages.clear();
	}

	~World() {

	}
};
