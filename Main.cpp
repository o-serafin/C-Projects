#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <math.h>

using namespace std;

#define LENGTH_OF_STRING 40
#define DIGITS_LENGTH 6 
#define NUMBER_OF_SHIP_CLASSES 4
#define NUMBER_OF_PLAYERS 2

struct field_t {
	char content = '.';
	int visible_to_A = 0;
	int visible_to_B = 0;
	int occupied_by_A = 0;
	int occupied_by_B = 0;
	int length_of_present_ship = 0;
	int is_head = 0;
	int is_tail = 0;
	int index_of_present_ship = 0; 
	int is_plane_A = 0;
	int is_plane_B = 0;
};

struct CAR {
	int is_placed = 0;
	int frontX = 0, fronY = 0, backX = 0, backY = 0;
	const int lenght = 5;
	char direction = 'a';
	int number_moves_in_given_turn = 0;
	int number_shoots = 0;
	int max_num_of_moves = 2;
	int has_engine = 1;
	int has_canon = 1;
	int has_radar = 1;
};

struct BAT {
	int is_placed = 0;
	int frontX = 0, fronY = 0, backX = 0, backY = 0;
	const int lenght = 4;
	char direction = 'a';
	int number_moves_in_given_turn = 0;
	int number_shoots = 0;
	int max_num_of_moves = 3;
	int has_engine = 1;
	int has_canon = 1;
	int has_radar = 1;
};

struct CRU {
	int is_placed = 0;
	int frontX = 0, fronY = 0, backX = 0, backY = 0;
	const int lenght = 3;
	char direction = 'a';
	int number_moves_in_given_turn = 0;
	int number_shoots = 0;
	int max_num_of_moves = 3;
	int has_engine = 1;
	int has_canon = 1;
	int has_radar = 1;
};

struct DES {
	int is_placed = 0;
	int frontX = 0, fronY = 0, backX = 0, backY = 0;
	const int lenght = 2;
	char direction = 'a';
	int number_moves_in_given_turn = 0;
	int number_shoots = 0;
	int max_num_of_moves = 3;
	int has_engine = 1;
	int has_canon = 1;
	int has_radar = 1;
};



int isCanon(field_t** board, int y, int x, int* sizeYptr, int* sizeXptr) {
	int is_canon = 0;
	for (int i = -1; i < 2;i++) {
		for (int j = -1; j < 2;j++) {
			if (i == 0 && j == 0) {
				continue;
			}
			else if (y + i >= 0 && y + i < *sizeYptr && x + j >= 0 && x + j < *sizeXptr && board[y + i][x + j].is_head == 1) {
				is_canon = 1;
			}
		}
	}
	return is_canon;
}



void printOneMechanismQuestion(int i, int j, int remainder2, int* sizeYptr, int* sizeXptr, field_t** board, int player) {
	if (j == remainder2) {
		cout << (i + 10) % 10; //printing vertical line closest to the board (censecutive digits from 0-9)
	}
	else if (j < remainder2) {
		cout << int(i / pow(10, remainder2 - j)); //printing printing any other line, depending on nomber of levels
	}
	else {//   [state]                            fields visible to [playerA]                                       fields visible to [playerB]
		if (player == 2 || (player == 0 && board[i][j - remainder2 - 1].visible_to_A) || (player == 1 && board[i][j - remainder2 - 1].visible_to_B)) {
			if (board[i][j - remainder2 - 1].is_head == 1 && board[i][j - remainder2 - 1].content == '+') {
				cout << '@';
			}
			else if (board[i][j - remainder2 - 1].is_tail == 1 && board[i][j - remainder2 - 1].content == '+') {
				cout << '%';
			}
			else if (board[i][j - remainder2 - 1].content == '+' && isCanon(board, i, j - remainder2 - 1, sizeYptr, sizeXptr)) {
				cout << '!';
			}
			else {
				cout << board[i][j - remainder2 - 1].content;
			}
		}
		else {
			cout << '?'; //if we print for player and field is out of his range of view
		}
	}
}

void printOneQuestion(int* sizeYptr, int* sizeXptr, field_t** board, int remainig_parts[2], int player, int from_state) {
	int remainder = log10(*sizeXptr - 1); // log of number of columns
	int remainder2 = log10(*sizeYptr - 1); //log of number of rows
	for (int i = 0; i < remainder + 1;i++) { //here we print top of our extended board
		for (int j = 0; j < *sizeXptr + remainder2 + 1; j++) {
			if (j < remainder2 + 1) {
				cout << ' '; //empty space at the beginning
			}
			else if (i == remainder) {
				cout << (j - remainder2 - 1 + 10) % 10; //printing horizontal line closest to the board (censecutive digits from 0-9)
			}
			else { //printing any other line, depending on nomber of levels
				cout << int((j - remainder2 - 1) / pow(10, remainder - i));
			}
		}
		cout << endl;
	}
	for (int i = 0; i < *sizeYptr;i++) {  //here we print content of extended board along with numbers meaning number of rows
		for (int j = 0; j < remainder2 + 1 + *sizeXptr; j++) {
			printOneMechanismQuestion(i, j, remainder2, sizeYptr, sizeXptr, board, player);
		}
		cout << endl;
	}
	if (from_state) { //if called by [state] command
		printf_s("PARTS REMAINING:: A : %d B : %d\n", remainig_parts[0], remainig_parts[1]);
	}
}
	



void printQuestion(field_t** board, int player, int sizeY, int sizeX, int remainig_parts[2], int from_state) {
	if (player == 2) { //printing for [state]
		for (int i = 0; i < sizeY; ++i) {
			for (int j = 0;j < sizeX;j++) {
				cout << board[i][j].content;
			}
			cout << endl;
		}
	}
	else {
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
				if (player == 1 && board[y][x].visible_to_B) { //printing for [playerB]
					cout << board[y][x].content;
				}
				else if (player == 0 && board[y][x].visible_to_A) { //printing for [playerA]
					cout << board[y][x].content;
				}
				else {
					cout << '?';
				}
			}
			cout << endl;
		}
	}
	if (from_state) {
		printf_s("PARTS REMAINING:: A : %d B : %d\n", remainig_parts[0], remainig_parts[1]);
	}
}



void printFunctionChoose(int *sizeYptr, int *sizeXptr, field_t**board, int remainig_parts[2]) {
	int a;
	cin >> a;
	if (a == 0) {
		printQuestion(board, 2, *sizeYptr, *sizeXptr, remainig_parts, 1);
	}
	else if (a == 1) {
		printOneQuestion(sizeYptr, sizeXptr, board, remainig_parts,2, 1);
	}
}



int playerIndex(char p) {
	int player = abs('B' - p - 1); // 0 or 1 retured
	return player;
}



void fleetAssign(int player, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES]) {
	for (int i = 0;i < 4;i++) {
		int a;
		cin >> a;
		if (a > 10) {
			break;
		}
		size_of_players[player][i] = a;
	}
}



int setFleetPlayer(int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES]) {
	char p;
	cin >> p;
	int player = playerIndex(p); // 0 or 1 retured
	fleetAssign(player, size_of_players);
	return player;
}



void nextPlayer(char *nextPtr) {
	char player;
	cin >> player;
	if (player != *nextPtr) {
		*nextPtr = player;
	}
}



field_t** createMap(int *sizeYptr, int *sizeXptr) {
	field_t** board = new field_t * [*sizeYptr];
	for (int i = 0; i < *sizeYptr; ++i) {
		board[i] = new field_t[*sizeXptr];
	}
	for (int i = 0; i < *sizeYptr; ++i) {
		for (int j = 0;j < *sizeXptr;j++) {
			board[i][j].content = ' ';
		}
	}
	return board;
}



void defineFleet(CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2], int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES]) {
	for (int i = 0;i < 4;i++) {
		int number;
		if (size_of_players[0][i] >= size_of_players[1][i]) {
			number = size_of_players[0][i];
		}
		else {
			number = size_of_players[1][i];
		}
		if (i == 0) {
			for (int j = 0;j < 2;j++) {
				carriers[j] = new CAR[number];
			}
		}
		else if (i == 1) {
			for (int j = 0;j < 2;j++) {
				battleships[j] = new BAT[number];
			}
		}
		else if (i == 2) {
			for (int j = 0;j < 2;j++) {
				cruisers[j] = new CRU[number];
			}
		}
		else if (i == 3) {
			for (int j = 0;j < 2;j++) {
				destroyers[j] = new DES[number];
			}
		}
	}
}



field_t** changeArr(field_t** arr, int* sizeYptr, int* sizeXptr) {
	int oldY = *sizeYptr;
	int newsizeY;
	cin >> newsizeY;
	*sizeYptr = newsizeY;
	int newsizeX;
	cin >> newsizeX;
	*sizeXptr = newsizeX;
	for (int i = 0; i < oldY; ++i) {
		delete[] arr[i];
	}
	delete[] arr;

	arr = new field_t* [*sizeYptr];
	for (int i = 0; i < *sizeYptr; ++i) {
		arr[i] = new field_t[*sizeXptr];
	}
	for (int i = 0; i < *sizeYptr; ++i) {
		for (int j = 0;j < *sizeXptr;j++) {
			arr[i][j].content = ' ';
		}
	}
	return arr;
}



void clearPlayer(int player, CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2]) {
	delete[] carriers[player];
	delete[] battleships[player];
	delete[] cruisers[player];
	delete[] destroyers[player];
}



void initPosition(int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES]) {
	char p;
	cin >> p;
	int player = playerIndex(p);
	for (int i = 0;i < 4;i++) {
		cin >> init_positions[player][i];
	}
}



void addReef(field_t** board, int* sizeYptr, int* sizeXptr, char state[LENGTH_OF_STRING]) {
	int y, x;
	cin >> y >> x;
	if (y < 0 || y >= *sizeYptr || x < 0 || x >= *sizeXptr) {
		printf_s("INVALID OPERATION \"%s %d %d\": REEF IS NOT PLACED ON BOARD", state, y, x);
		exit(1);
	}
	board[y][x].content = '#';
}



void exceededClass(int i, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int player, int claas_num, char move_player_command[LENGTH_OF_STRING], int y, int x, char direction, char clas[4]) {
	if (i >= size_of_players[player][claas_num]) { //class_num is an index of a class (0 for CAR, 1 for BAT, 2 for CRU, 3 for DES)
		printf_s("INVALID OPERATION \"%s %d %d %c %d %s\": ALL SHIPS OF THE CLASS ALREADY SET", move_player_command, y, x, direction, i, clas);
		exit(1);
	}
}



void shipAlreadyPresent(int is_already_placed, int i, char move_player_command[LENGTH_OF_STRING], int y, int x, char direction, char clas[4]) {
	if (is_already_placed == 1) { //individual parameter of given ship
		printf_s("INVALID OPERATION \"%s %d %d %c %d %s\": SHIP ALREADY PRESENT", move_player_command, y, x, direction, i, clas);
		exit(1);
	}
}



void increaseParts(int length, int ramaining_parts[2], int player, char digits[DIGITS_LENGTH]) {
	for (int i = 0; i < length; i++) {
		if (digits[i] == '1') {
			ramaining_parts[player] += 1;
		}
	}
}



void toCloseToShip(char command[LENGTH_OF_STRING], int y, int x, int i, char clas[4], char dir, int was_from, char digits[DIGITS_LENGTH], int player) { //was_from 1 for ship, 0 for move 2 for place_ship
	if (was_from == 1) {  // 1 means it was from SHIP command
		char playerchar = 'B';
		if (player == 0) {
			playerchar = 'A';
		}
		printf_s("INVALID OPERATION \"%s %c %d %d %c %d %s %s\": PLACING SHIP TOO CLOSE TO OTHER SHIP", command, playerchar, y, x, dir, i, clas, digits);
	}
	
	else if (was_from == 2) { // 2 means it was from PLACE_SHIP command
		printf_s("INVALID OPERATION \"%s %d %d %c %d %s\": PLACING SHIP TOO CLOSE TO OTHER SHIP", command, y, x, dir, i, clas);
	}
	else { //remaining option is 0 for move command
		printf_s("INVALID OPERATION \"%s %d %s %c\": PLACING SHIP TOO CLOSE TO OTHER SHIP", command, i, clas, dir);
	}
	exit(1);
}



void placeOnReef(char command[LENGTH_OF_STRING], int y, int x, int i, char clas[4], char dir, int was_from, char digits[DIGITS_LENGTH], int player) {
	if (was_from == 1) { // 1 means it was from SHIP command
		char playerchar = 'B';
		if (player == 0) {
			playerchar = 'A';
		}
		printf_s("INVALID OPERATION \"%s %c %d %d %c %d %s %s\": PLACING SHIP ON REEF", command, playerchar, y, x, dir, i, clas, digits);
	}
	else if (was_from == 2){ // 2 means it was from PLACE_SHIP command
		printf_s("INVALID OPERATION \"%s %d %d %c %d %s\": PLACING SHIP ON REEF", command, y, x, dir, i, clas);
	}
	else { //remaining option is 0 for move command
		printf_s("INVALID OPERATION \"%s %d %s %c\": PLACING SHIP ON REEF", command, i, clas, dir);
	}
	exit(1);
}



void notToCloseToOtherShipOrReef(int minY, int minX, int maxY, int maxX, field_t** board, int* ptrYsize, int* ptrXsize, int was_from_place_ship, int y, int x, int index, char clas[4], char command[LENGTH_OF_STRING], char dir, char digits[DIGITS_LENGTH], int player) {
	for (int i = minY - 1; i <= maxY + 1; i++) {
		for (int j = minX - 1; j <= maxX + 1; j++) {
			if (i < 0 || j < 0 || i >= *ptrYsize || j >= *ptrXsize) {
				continue;
			}
			if (board[i][j].content != '#' && board[i][j].content != ' ' && board[i][j].content != '.') {
				toCloseToShip(command, y, x, index, clas, dir, was_from_place_ship, digits, player);
			}
			if (i >= minY && i <= maxY && j >= minX && j <= maxX && board[i][j].content == '#') { //only on the position of ship, not around it
				placeOnReef(command, y, x, index, clas, dir, was_from_place_ship, digits, player);
			}
		}
	}
}



void placeOnPositionReversed(int minY, int minX, int maxY, int maxX, field_t** board, int length, char digits[DIGITS_LENGTH], int player, int index) {
	int iteration = 0;
	for (int i = maxY; i >= minY; i--) {
		for (int j = maxX; j >= minX; j--) {
			if (i == maxY && j == maxX) {
				board[i][j].is_head = 1;
			}
			else if (i == minY && j == minX) {
				board[i][j].is_tail = 1;
			}
			if (digits[iteration] == '1') {
				board[i][j].content = '+';
			}
			else {
				board[i][j].content = 'x';
			}
			board[i][j].length_of_present_ship = length;
			board[i][j].index_of_present_ship = index;
			if (player == 0) {
				board[i][j].occupied_by_A = 1;
			}
			else {
				board[i][j].occupied_by_B = 1;
			}
			iteration += 1;
		}
	}
}



void placeOnPosition(int minY, int minX, int maxY, int maxX, field_t** board, int length, char digits[DIGITS_LENGTH], int player, int index) {
	int iteration = 0;
	for (int i = minY; i <= maxY; i++) {
		for (int j = minX; j <= maxX; j++) {
			if (i == minY && j == minX) {
				board[i][j].is_head = 1;
			}
			else if (i == maxY && j == maxX) {
				board[i][j].is_tail = 1;
			}
			if (digits[iteration] == '1') {
				board[i][j].content = '+';
			}
			else {
				board[i][j].content = 'x';
			}
			board[i][j].length_of_present_ship = length;
			board[i][j].index_of_present_ship = index;
			if (player == 0) {
				board[i][j].occupied_by_A = 1;
			}
			else {
				board[i][j].occupied_by_B = 1;
			}
			iteration += 1;
		}
	}
}



void placeOnBoardAlmost(field_t** board, int y, int x, int ptr_tailY, int ptr_tailX, int player, int length, int remainig_parts[2], char digits[DIGITS_LENGTH], int* sizeYptr, int* sizeXptr, int if_remaining, int i, int was_from_place_ship, char clas[4], char command[LENGTH_OF_STRING], char dir) {
	int minY, minX, maxY, maxX, is_reversed = 0;
	if (y > ptr_tailY) {
		is_reversed = 1;
		maxY = y;
		minY = ptr_tailY;
	}
	else {
		maxY = ptr_tailY;
		minY = y;
	}
	if (x > ptr_tailX) {
		is_reversed = 1;
		maxX = x;
		minX = ptr_tailX;
	}
	else {
		maxX = ptr_tailX;
		minX = x;
	}
	notToCloseToOtherShipOrReef(minY, minX, maxY, maxX, board, sizeYptr, sizeXptr, was_from_place_ship, y, x, i, clas, command, dir, digits, player);
	if (!is_reversed) {
		placeOnPosition(minY, minX, maxY, maxX, board, length, digits, player, i); 
	}
	else {
		placeOnPositionReversed(minY, minX, maxY, maxX, board, length, digits, player, i);
	}
	if (if_remaining) {
		increaseParts(length, remainig_parts, player, digits);
	}
}



void notInitPosition(int i, int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int player, char move_player_command[LENGTH_OF_STRING], int y, int x, char direction, char clas[4], int* ptr_tailY, int* ptr_tailX) {
	if (y < init_positions[player][0] || *ptr_tailY < init_positions[player][0] || y > init_positions[player][2] || *ptr_tailY > init_positions[player][2] || x < init_positions[player][1] || *ptr_tailX < init_positions[player][1] || x > init_positions[player][3] || *ptr_tailX > init_positions[player][3]) {
		printf_s("INVALID OPERATION \"%s %d %d %c %d %s\": NOT IN STARTING POSITION", move_player_command, y, x, direction, i, clas);
		exit(1);
	}
}



void setPositionShip(int length, int i, char direction, int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], char move_player_command[LENGTH_OF_STRING], int y, int x, char clas[4], int* ptr_tailY, int* ptr_tailX, int player, field_t** board, int remainig_parts[2], char digits[DIGITS_LENGTH], int musit_check_init_position, int* sizeYptr, int* sizeXptr, int was_from_place_ship) {
	if (direction == 'N') {
		*ptr_tailY = (y + length - 1);
	}
	else if (direction == 'S') {
		*ptr_tailY = (y - length + 1);
	}
	else if (direction == 'E') {
		*ptr_tailX = (x - length + 1);
	}
	else if (direction == 'W') {
		*ptr_tailX = (x + length - 1);
	}
	if (musit_check_init_position == 1) {
		notInitPosition(i, init_positions, player, move_player_command, y, x, direction, clas, ptr_tailY, ptr_tailX);
	}
	placeOnBoardAlmost(board, y, x, *ptr_tailY, *ptr_tailX, player, length, remainig_parts, digits, sizeYptr, sizeXptr, 1, i, was_from_place_ship, clas, move_player_command, direction);
}



void setParemetersCarr(CAR* carriers[2], int i, int player, char digits[DIGITS_LENGTH]) {
	for (int j = 0; j < 5;j++) {
		if (j == 0 && digits[j] == '0') {
			carriers[player][i].has_radar = 0;
		}
		if (j == 4 && digits[j] == '0') {
			carriers[player][i].has_engine = 0;
		}
		if (j == 1 && digits[j] == '0') {
			carriers[player][i].has_canon = 0;
		}
	}
}



void setParemetersBat(BAT* battleships[2], int i, int player, char digits[DIGITS_LENGTH]) {
	for (int j = 0; j < 4;j++) {
		if (j == 0 && digits[j] == '0') {
			battleships[player][i].has_radar = 0;
		}
		if (j == 3 && digits[j] == '0') {
			battleships[player][i].has_engine = 0;
		}
		if (j == 1 && digits[j] == '0') {
			battleships[player][i].has_canon = 0;
		}
	}
}



void setParemetersCru(CRU* cruisers[2], int i, int player, char digits[DIGITS_LENGTH]) {
	for (int j = 0; j < 3;j++) {
		if (j == 0 && digits[j] == '0') {
			cruisers[player][i].has_radar = 0;
		}
		if (j == 2 && digits[j] == '0') {
			cruisers[player][i].has_engine = 0;
		}
		if (j == 1 && digits[j] == '0') {
			cruisers[player][i].has_canon = 0;
		}
	}
}



void setParemetersDes(DES* destroyers[2], int i, int player, char digits[DIGITS_LENGTH]) {
	for (int j = 0; j < 2;j++) {
		if (j == 0 && digits[j] == '0') {
			destroyers[player][i].has_radar = 0;
		}
		if (j == 1 && digits[j] == '0') {
			destroyers[player][i].has_engine = 0;
			destroyers[player][i].has_canon = 0;
		}
	}
}



void findShipCarr(char clas[4], int i, int player, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], CAR* carriers[2], int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], char move_player_command[LENGTH_OF_STRING], int y, int x, char direction, field_t** board, int ships_already_present[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int remainig_parts[2], char digits[DIGITS_LENGTH], int must_check_init_position, int* ptr_tailY, int* ptr_tailX, int* sizeYptr, int* sizeXptr, int was_from_place_ship) {
	exceededClass(i, size_of_players, player, 0, move_player_command, y, x, direction, clas);
	shipAlreadyPresent(carriers[player][i].is_placed, i, move_player_command, y, x, direction, clas);
	setPositionShip(5, i, direction, init_positions, move_player_command, y, x, clas, ptr_tailY, ptr_tailX, player, board, remainig_parts, digits, must_check_init_position, sizeYptr, sizeXptr, was_from_place_ship);
	carriers[player][i].is_placed = 1;
	carriers[player][i].backX = *ptr_tailX;
	carriers[player][i].backY = *ptr_tailY;
	carriers[player][i].fronY = y;
	carriers[player][i].frontX = x;
	carriers[player][i].direction = direction;
	ships_already_present[player][0] += 1;
	setParemetersCarr(carriers, i, player, digits);
}



void findShipBat(char clas[4], int i, int player, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], BAT* battleships[2], int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], char move_player_command[LENGTH_OF_STRING], int y, int x, char direction, field_t** board, int ships_already_present[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int remainig_parts[2], char digits[DIGITS_LENGTH], int must_check_init_position, int* ptr_tailY, int* ptr_tailX, int* sizeYptr, int* sizeXptr, int was_from_place_ship) {
	exceededClass(i, size_of_players, player, 1, move_player_command, y, x, direction, clas);
	shipAlreadyPresent(battleships[player][i].is_placed, i, move_player_command, y, x, direction, clas);
	setPositionShip(4, i, direction, init_positions, move_player_command, y, x, clas, ptr_tailY, ptr_tailX, player, board, remainig_parts, digits, must_check_init_position, sizeYptr, sizeXptr, was_from_place_ship);
	battleships[player][i].is_placed = 1;
	battleships[player][i].backX = *ptr_tailX;
	battleships[player][i].backY = *ptr_tailY;
	battleships[player][i].fronY = y;
	battleships[player][i].frontX = x;
	battleships[player][i].direction = direction;
	ships_already_present[player][1] += 1;
	setParemetersBat(battleships, i, player, digits);
}



void findShipCru(char clas[4], int i, int player, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], CRU* cruisers[2], int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], char move_player_command[LENGTH_OF_STRING], int y, int x, char direction, field_t** board, int ships_already_present[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int remainig_parts[2], char digits[DIGITS_LENGTH], int must_check_init_position, int* ptr_tailY, int* ptr_tailX, int* sizeYptr, int* sizeXptr, int was_from_place_ship) {
	exceededClass(i, size_of_players, player, 2, move_player_command, y, x, direction, clas);
	shipAlreadyPresent(cruisers[player][i].is_placed, i, move_player_command, y, x, direction, clas);
	setPositionShip(3, i, direction, init_positions, move_player_command, y, x, clas, ptr_tailY, ptr_tailX, player, board, remainig_parts, digits, must_check_init_position, sizeYptr, sizeXptr, was_from_place_ship);
	cruisers[player][i].is_placed = 1;
	cruisers[player][i].backX = *ptr_tailX;
	cruisers[player][i].backY = *ptr_tailY;
	cruisers[player][i].fronY = y;
	cruisers[player][i].frontX = x;
	cruisers[player][i].direction = direction;
	ships_already_present[player][2] += 1;
	setParemetersCru(cruisers, i, player, digits);
}



void findShipDes(char clas[4], int i, int player, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], DES* destroyers[2], int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], char move_player_command[LENGTH_OF_STRING], int y, int x, char direction, field_t** board, int ships_already_present[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int remainig_parts[2], char digits[DIGITS_LENGTH], int must_check_init_position, int* ptr_tailY, int* ptr_tailX, int* sizeYptr, int* sizeXptr, int was_from_place_ship) {
	exceededClass(i, size_of_players, player, 3, move_player_command, y, x, direction, clas);
	shipAlreadyPresent(destroyers[player][i].is_placed, i, move_player_command, y, x, direction, clas);
	setPositionShip(2, i, direction, init_positions, move_player_command, y, x, clas, ptr_tailY, ptr_tailX, player, board, remainig_parts, digits, must_check_init_position, sizeYptr, sizeXptr, was_from_place_ship);
	destroyers[player][i].is_placed = 1;
	destroyers[player][i].backX = *ptr_tailX;
	destroyers[player][i].backY = *ptr_tailY;
	destroyers[player][i].fronY = y;
	destroyers[player][i].frontX = x;
	destroyers[player][i].direction = direction;
	ships_already_present[player][3] += 1;
	setParemetersDes(destroyers, i, player, digits);
}



void findShip(char clas[4], int i, int player, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2], int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], char move_player_command[LENGTH_OF_STRING], int y, int x, char direction, field_t** board, int ships_already_present[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int remainig_parts[2], char digits[DIGITS_LENGTH], int must_check_init_position, int* sizeYptr, int* sizeXptr, int was_from_place_ship) {
	int* ptr_tailY = &y;
	int* ptr_tailX = &x;
	if (strcmp(clas, "CAR") == 0) {
		findShipCarr(clas, i, player, size_of_players, carriers, init_positions, move_player_command, y, x, direction, board, ships_already_present, remainig_parts, digits, must_check_init_position, ptr_tailY, ptr_tailX, sizeYptr, sizeXptr, was_from_place_ship);
	}
	else if (strcmp(clas, "BAT") == 0) {
		findShipBat(clas, i, player, size_of_players, battleships, init_positions, move_player_command, y, x, direction, board, ships_already_present, remainig_parts, digits, must_check_init_position, ptr_tailY, ptr_tailX, sizeYptr, sizeXptr, was_from_place_ship);
	}
	else if (strcmp(clas, "CRU") == 0) {
		findShipCru(clas, i, player, size_of_players, cruisers, init_positions, move_player_command, y, x, direction, board, ships_already_present, remainig_parts, digits, must_check_init_position, ptr_tailY, ptr_tailX, sizeYptr, sizeXptr, was_from_place_ship);
	}
	else if (strcmp(clas, "DES") == 0) {
		findShipDes(clas, i, player, size_of_players, destroyers, init_positions, move_player_command, y, x, direction, board, ships_already_present, remainig_parts, digits, must_check_init_position, ptr_tailY, ptr_tailX, sizeYptr, sizeXptr, was_from_place_ship);
	}
}



void shipCommand(char move_player_command[LENGTH_OF_STRING], field_t** board, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2], int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int ships_already_present[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int remainig_parts[2], int* sizeYptr, int* sizeXptr) {
	int y, x, i;
	char direction, p;
	char clas[4], digits[DIGITS_LENGTH];
	cin >> p >> y >> x >> direction >> i >> clas >> digits;
	int player = playerIndex(p);
	findShip(clas, i, player, size_of_players, carriers, battleships, cruisers, destroyers, init_positions, move_player_command, y, x, direction, board, ships_already_present, remainig_parts, digits, 0, sizeYptr, sizeXptr,1);
}



void composeDigitsNorth(int length, field_t** board, int frontY, int frontX) {
	for (int y = frontY; y < frontY + length; y++) {
		if (board[y][frontX].content == '+') {
			cout << 1;
		}
		else {
			cout << 0;
		}
	}
	cout << endl;
}



void composeDigitsEast(int length, field_t** board, int frontY, int frontX) {
	for (int x = frontX; x > frontX - length; x--) {
		if (board[frontY][x].content == '+') {
			cout << 1;
		}
		else {
			cout << 0;
		}
	}
	cout << endl;
}



void composeDigitsWest(int length, field_t** board, int frontY, int frontX) {
	for (int x = frontX; x < frontX + length; x++) {
		if (board[frontY][x].content == '+') {
			cout << 1;
		}
		else {
			cout << 0;
		}
	}
	cout << endl;
}



void composeDigitsSouth(int length, field_t** board, int frontY, int frontX) {
	for (int y = frontY; y > frontY - length; y--) {
		if (board[y][frontX].content == '+') {
			cout << 1;
		}
		else {
			cout << 0;
		}
	}
	cout << endl;
}



void composeDigits(char direction, int length, field_t** board, int frontY, int frontX) {
	if (direction == 'N') {
		composeDigitsNorth(length, board, frontY, frontX);
	}
	else if (direction == 'E') {
		composeDigitsEast(length, board, frontY, frontX);
	}
	else if (direction == 'S') {
		composeDigitsSouth(length, board, frontY, frontX);
	}
	else if (direction == 'W') {
		composeDigitsWest(length, board, frontY, frontX);
	}
}



void playerPrintSave(int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2], int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], char nextePtr, field_t** board) {
	int index = playerIndex(nextePtr);
	cout << "INIT_POSITION "<< nextePtr<< " " << init_positions[index][0] << " " << init_positions[index][1] << " " << init_positions[index][2] << " " << init_positions[index][3] << endl;
	cout << "SET_FLEET " << nextePtr<< " " << size_of_players[index][0] << " " << size_of_players[index][1] << " " << size_of_players[index][2] << " " << size_of_players[index][3] << endl;
	for (int i = 0; i < size_of_players[index][0];i++) {
		cout << "SHIP " << nextePtr << " " << carriers[index][i].fronY << " " << carriers[index][i].frontX << " " << carriers[index][i].direction << " " << i << " " << "CAR ";
		composeDigits(carriers[index][i].direction, 5, board, carriers[index][i].fronY, carriers[index][i].frontX);
	}
	for (int i = 0; i < size_of_players[index][1];i++) {
		cout << "SHIP " << nextePtr << " " << battleships[index][i].fronY << " " << battleships[index][i].frontX << " " << battleships[index][i].direction << " " << i << " " << "BAT ";
		composeDigits(battleships[index][i].direction, 4, board, battleships[index][i].fronY, battleships[index][i].frontX);
	}
	for (int i = 0; i < size_of_players[index][2];i++) {
		cout << "SHIP " << nextePtr << " " << cruisers[index][i].fronY << " " << cruisers[index][i].frontX << " " << cruisers[index][i].direction << " " << i << " " << "CRU ";
		composeDigits(cruisers[index][i].direction, 3, board, cruisers[index][i].fronY, cruisers[index][i].frontX);
	}
	for (int i = 0; i < size_of_players[index][3];i++) {
		cout << "SHIP " << nextePtr << " " << destroyers[index][i].fronY << " " << destroyers[index][i].frontX << " " << destroyers[index][i].direction << " " << i << " " << "DES ";
		composeDigits(destroyers[index][i].direction, 2, board, destroyers[index][i].fronY, destroyers[index][i].frontX);
	}
}


void printReefs(int* sizeYptr, int* sizeXptr, field_t** board) {
	for (int y = 0; y < *sizeYptr; y++) {
		for (int x = 0; x < *sizeXptr; x++) {
			if (board[y][x].content == '#') {
				cout << "REEF " << y << " " << x << endl;
			}
		}
	}
}


void saveFunct(int* sizeYptr, int* sizeXptr, char* nextePtr, field_t** board, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2], int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int* is_game_extended) {
	cout << "[state]" << endl;
	cout << "BOARD_SIZE " << *sizeYptr << " " << *sizeXptr << endl;
	cout << "NEXT_PLAYER " << *nextePtr << endl;
	playerPrintSave(size_of_players, carriers, battleships, cruisers, destroyers, init_positions, 'A', board);
	playerPrintSave(size_of_players, carriers, battleships, cruisers, destroyers, init_positions, 'B', board);
	printReefs(sizeYptr, sizeXptr, board);
	if (*is_game_extended == 1) {
		cout << "EXTENDED_SHIPS" << endl;
	}
	cout << "[state]" << endl;
}



field_t** state(int *sizeYptr,int *sizeXptr,char *nextePtr, field_t** board, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2],int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int remainig_parts[2], int ships_already_present[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int* is_game_extended) {
	char state_command[LENGTH_OF_STRING] = "sth";
	while (strcmp(state_command, "[state]") != 0) {
		cin >> state_command;
		if (strcmp(state_command, "PRINT") == 0) {
			printFunctionChoose(sizeYptr, sizeXptr, board, remainig_parts);
		}
		else if (strcmp(state_command, "SET_FLEET") == 0) {
			int player = setFleetPlayer(size_of_players);
			clearPlayer(player, carriers, battleships, cruisers, destroyers);
			defineFleet(carriers, battleships, cruisers, destroyers, size_of_players);
		}
		else if (strcmp(state_command, "NEXT_PLAYER") == 0) {
			nextPlayer(nextePtr);
		}
		else if (strcmp(state_command, "INIT_POSITION") == 0) {
			initPosition(init_positions);
		}
		else if (strcmp(state_command, "BOARD_SIZE") == 0) {
			board = changeArr(board, sizeYptr, sizeXptr);
		}
		else if (strcmp(state_command, "REEF") == 0) {
			addReef(board,sizeYptr,sizeXptr,state_command);
		}
		else if (strcmp(state_command, "SHIP") == 0) {
			shipCommand(state_command, board, size_of_players, carriers, battleships, cruisers, destroyers, init_positions, ships_already_present, remainig_parts, sizeYptr, sizeXptr);
		}
		else if (strcmp(state_command, "EXTENDED_SHIPS") == 0) {
			*is_game_extended = 1;
		}
		else if (strcmp(state_command, "SAVE") == 0) {
			saveFunct(sizeYptr, sizeXptr, nextePtr, board, size_of_players, carriers, battleships, cruisers, destroyers, init_positions, is_game_extended);
		}
	}
	return board;
}



void placeShip(int player, char move_player_command[LENGTH_OF_STRING], field_t** board, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2], int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int ships_already_present[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int remainig_parts[2], int* sizeYptr, int* sizeXptr) {
	int y, x,i;
	char direction;
	char clas[4];
	cin >> y >> x >> direction >>i>> clas;
	char digits[DIGITS_LENGTH] = "11111";
	findShip(clas, i, player, size_of_players, carriers, battleships, cruisers, destroyers, init_positions, move_player_command, y, x, direction, board, ships_already_present, remainig_parts, digits, 1, sizeYptr, sizeXptr, 2);
}



void turnOffRadarShip(int player, CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2], int length_of_present_ship, int index_of_ship) {
	if (length_of_present_ship == 5) {
		carriers[player][index_of_ship].has_radar = 0;
	}
	else if (length_of_present_ship == 4) {
		battleships[player][index_of_ship].has_radar = 0;
	}
	else if (length_of_present_ship == 3) {
		cruisers[player][index_of_ship].has_radar = 0;
	}
	else if (length_of_present_ship == 2) {
		destroyers[player][index_of_ship].has_radar = 0;
	}
}



void turnOffEngineShip(int player, CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2], int length_of_present_ship, int index_of_ship) {
	if (length_of_present_ship == 5) {
		carriers[player][index_of_ship].has_engine = 0;
	}
	else if (length_of_present_ship == 4) {
		battleships[player][index_of_ship].has_engine = 0;
	}
	else if (length_of_present_ship == 3) {
		cruisers[player][index_of_ship].has_engine = 0;
	}
	else if (length_of_present_ship == 2) {
		destroyers[player][index_of_ship].has_engine = 0;
		destroyers[player][index_of_ship].has_canon = 0;
	}
}



void turnOffCanonShip(int player, CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], int length_of_present_ship, int index_of_ship) {
	if (length_of_present_ship == 5) {
		carriers[player][index_of_ship].has_canon = 0;
	}
	else if (length_of_present_ship == 4) {
		battleships[player][index_of_ship].has_canon = 0;
	}
	else if (length_of_present_ship == 3) {
		cruisers[player][index_of_ship].has_canon = 0;
	}
}



int shipWasHitAdvanced(field_t** board, int y, int x, CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2], int* sizeYptr, int* sizeXptr) {
	char tile = board[y][x].content; // this command is used also for basic shooting to save space because it does not affect basic logic of the game (there are no radars, engines or canons)
	if (tile == '+') {
		if (board[y][x].is_head) {
			turnOffRadarShip(board[y][x].occupied_by_B, carriers, battleships, cruisers, destroyers, board[y][x].length_of_present_ship, board[y][x].index_of_present_ship);
		}
		else if (board[y][x].is_tail) {
			turnOffEngineShip(board[y][x].occupied_by_B, carriers, battleships, cruisers, destroyers, board[y][x].length_of_present_ship, board[y][x].index_of_present_ship);
		}
		else if (isCanon(board, y, x, sizeYptr, sizeXptr)) {
			turnOffCanonShip(board[y][x].occupied_by_B, carriers, battleships, cruisers, board[y][x].length_of_present_ship, board[y][x].index_of_present_ship);
		}
		board[y][x].content = 'x';
		return 1;
	}
	return 0;
}



void notAllShipsPresent(int y, int x, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int ships_already_present[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], char move_player_command[LENGTH_OF_STRING]) {
	for (int i = 0; i < 2;i++) {
		for (int j = 0;j < 4;j++) {
			if (size_of_players[i][j] != ships_already_present[i][j]) {
				printf_s("INVALID OPERATION \"%s %d %d\": NOT ALL SHIPS PLACED", move_player_command, y, x);
				exit(1);
			}
		}
	}
}



void beyondBoard(int y, int x, int* sizeYptr, int* sizeXptr, char move_player_command[LENGTH_OF_STRING]) {
	if (y > *sizeYptr - 1 || y < 0 || x > *sizeXptr - 1 || x < 0) {
		printf_s("INVALID OPERATION \"%s %d %d\": FIELD DOES NOT EXIST", move_player_command, y, x);
		exit(1);
	}
}



void decreaseRemainingParts(int remaining_parts[2], field_t** board, int y, int x){
	if (board[y][x].occupied_by_A == 1) {
		remaining_parts[0] -= 1;
	}
	else {
		remaining_parts[1] -= 1;
	}
	if (remaining_parts[0] == 0) {
		cout << "B won";
		exit(1);
	}
	if (remaining_parts[1] == 0) {
		cout << "A won";
		exit(1);
	}
	
}



void shootFunct(field_t** board, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int ships_already_present[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], char move_player_command[LENGTH_OF_STRING], int* sizeYptr, int* sizeXptr, int remaining_parts[2], CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2]) {
	int y, x;
	cin >> y >> x;
	notAllShipsPresent(y, x, size_of_players, ships_already_present, move_player_command);
	beyondBoard(y, x, sizeYptr, sizeXptr, move_player_command);
	if (shipWasHitAdvanced(board, y, x, carriers, battleships, cruisers, destroyers, sizeYptr, sizeXptr)) {
		decreaseRemainingParts(remaining_parts, board, y, x);
	}
}



void hasEngineAndFreeMoves(int has_engine, int moves_made,int max_moves, int i, char clas[4], char dir){
	if (!has_engine) {
		printf_s("INVALID OPERATION \"MOVE %d %s %c\": SHIP CANNOT MOVE", i, clas, dir);
		exit(1);
	}
	else if (moves_made == max_moves) {  //individual for every given ship
		printf_s("INVALID OPERATION \"MOVE %d %s %c\": SHIP MOVED ALREADY", i, clas, dir);
		exit(1);
	}
}



void newPositionNorth(int tab[4], int length, char dir, int newe_tab[4], char* newe_direction_ship) { //tab[0]=frontY  tab[1]=frontX  tab[2]=backY  tab[3]=backX
	if (dir == 'L') {
		newe_tab[1] = tab[1] - length + 1;
		newe_tab[2] = tab[2] - length;
		*newe_direction_ship = 'W';
	}
	else if (dir == 'F') {
		newe_tab[1] = tab[1];
		newe_tab[2] = tab[2] - 1;
		*newe_direction_ship = 'N';
	}
	else if (dir == 'R') {
		newe_tab[1] = tab[1] + length - 1;
		newe_tab[2] = tab[2] - length;
		*newe_direction_ship = 'E';
	}
	newe_tab[0] = tab[0] - 1;
	newe_tab[3] = tab[3];
}



void newPositionEast(int tab[4], int length, char dir, int newe_tab[4], char* newe_direction_ship) { //tab[0]=frontY  tab[1]=frontX  tab[2]=backY  tab[3]=backX
	if (dir == 'L') {
		newe_tab[0] = tab[0] - length + 1;
		newe_tab[3] = tab[3] + length;
		*newe_direction_ship = 'N';
	}
	else if (dir == 'F') {
		newe_tab[0] = tab[0];
		newe_tab[3] = tab[3] + 1;
		*newe_direction_ship = 'E';
	}
	else if (dir == 'R') {
		newe_tab[0] = tab[0] + length - 1;
		newe_tab[3] = tab[3] + length;
		*newe_direction_ship = 'S';
	}
	newe_tab[1] = tab[1] + 1;
	newe_tab[2] = tab[2];
}



void newPositionSouth(int tab[4], int length, char dir, int newe_tab[4], char* newe_direction_ship) { //tab[0]=frontY  tab[1]=frontX  tab[2]=backY  tab[3]=backX
	if (dir == 'L') {
		newe_tab[1] = tab[1] + length - 1;
		newe_tab[2] = tab[2] + length;
		*newe_direction_ship = 'E';
	}
	else if (dir == 'F') {
		newe_tab[1] = tab[1];
		newe_tab[2] = tab[2] + 1;
		*newe_direction_ship = 'S';
	}
	else if (dir == 'R') {
		newe_tab[1] = tab[1] - length + 1;
		newe_tab[2] = tab[2] + length;
		*newe_direction_ship = 'W';
	}
	newe_tab[0] = tab[0] + 1;
	newe_tab[3] = tab[3];

}



void newPositionWest(int tab[4], int length, char dir, int newe_tab[4], char* newe_direction_ship) { //tab[0]=frontY  tab[1]=frontX  tab[2]=backY  tab[3]=backX
	if (dir == 'L') {
		newe_tab[0] = tab[0] + length - 1;
		newe_tab[3] = tab[3] - length;
		*newe_direction_ship = 'S';
	}
	else if (dir == 'F') {
		newe_tab[0] = tab[0];
		newe_tab[3] = tab[3] - 1;
		*newe_direction_ship = 'W';
	}
	else if (dir == 'R') {
		newe_tab[0] = tab[0] - length + 1;
		newe_tab[3] = tab[3] - length;
		*newe_direction_ship = 'N';
	}
	newe_tab[1] = tab[1] - 1;
	newe_tab[2] = tab[2];
}



void shipWentFromBoard(int sizeYptr, int sizeXptr, int newe_tab[4], int j, char clas[4], char dir) {
	for (int i = 0; i < 4; i++) {
		if (newe_tab[i] < 0) {
			printf_s("INVALID OPERATION \"MOVE %d %s %c\": SHIP WENT FROM BOARD", j, clas, dir);
			exit(1);
		}
		if (newe_tab[i] >= sizeYptr && (i == 0 || i == 2)) {
			cout << sizeYptr << " " << sizeXptr << endl;
			printf_s("INVALID OPERATION \"MOVE %d %s %c\": SHIP WENT FROM BOARD", j, clas, dir);
			exit(1);
		}
		if (newe_tab[i] >= sizeXptr  && (i == 1 || i == 3 ) ) {
			printf_s("INVALID OPERATION \"MOVE %d %s %c\": SHIP WENT FROM BOARD", j, clas, dir);
			exit(1);
		}
	}
}


void newPosition(int tab[4], int length, char ship_direction, char dir, field_t** board, int i, char clas[4], int newe_tab[4], int sizeYptr, int sizeXptr, char* newe_direction_ship ) { //tab [0] = frontY [1] = frontX
	if (ship_direction == 'N') {
		newPositionNorth(tab, length, dir, newe_tab, newe_direction_ship);
	}
	else if (ship_direction == 'E') {
		newPositionEast(tab, length, dir, newe_tab, newe_direction_ship);
	}
	else if (ship_direction == 'S') {
		newPositionSouth(tab, length, dir, newe_tab, newe_direction_ship);
	}
	else if (ship_direction == 'W') {
		newPositionWest(tab, length, dir, newe_tab, newe_direction_ship);
	}
	shipWentFromBoard(sizeYptr, sizeXptr, newe_tab, i, clas, dir); // after calculating new position checking if it is on board
	
}



void clearFinallyReversed(field_t** board, int maxY, int maxX, int minY, int minX, char digits[DIGITS_LENGTH]) {
	int iteration = 0;
	for (int i = maxY; i >= minY; i--) {
		for (int j = maxX; j >= minX; j--) {
			if (board[i][j].content == '+') {
				digits[iteration] = '1';
			}
			board[i][j].content = ' ';
			board[i][j].length_of_present_ship = 0;
			board[i][j].occupied_by_A = 0;
			board[i][j].occupied_by_B = 0;
			board[i][j].is_head = 0;
			board[i][j].is_tail = 0;
			board[i][j].index_of_present_ship = 0;
			iteration += 1;
		}
	}
}



void clearFinally(field_t** board, int maxY, int maxX, int minY, int minX, char digits[DIGITS_LENGTH]) {
	int iteration = 0;
	for (int i = minY; i <= maxY;i++) {
		for (int j = minX; j <= maxX; j++) {
			if (board[i][j].content == '+') {
				digits[iteration] = '1';
			}
			board[i][j].content = ' ';
			board[i][j].length_of_present_ship = 0;
			board[i][j].occupied_by_A = 0;
			board[i][j].occupied_by_B = 0;
			board[i][j].is_head = 0;
			board[i][j].is_tail = 0;
			board[i][j].index_of_present_ship = 0;
			iteration += 1;
		}
	}
}



void clearPreviousPosition(field_t** board, int frontY, int frontX, int backY, int backX, char digits[DIGITS_LENGTH]) {
	int maxY, maxX, minY, minX, is_reversed = 0;
	if (frontY > backY) {
		is_reversed = 1;
		maxY = frontY;
		minY = backY;
	}
	else {
		maxY = backY;
		minY = frontY;
	}
	if (frontX > backX) {
		is_reversed = 1;
		maxX = frontX;
		minX = backX;
	}
	else {
		maxX = backX;
		minX = frontX;
	}
	if (is_reversed) {
		clearFinallyReversed(board, maxY, maxX, minY, minX, digits);
	}
	else {
		clearFinally(board, maxY, maxX, minY, minX, digits); //maxY, maxX, minY, minX,
	}
}



void defineTab(int tab[4], int a, int b, int c, int d) { //it is just for help
	tab[0] = a;
	tab[1] = b;
	tab[2] = c;
	tab[3] = d;
}



void movePlayerFrameCarr(int player, char main_command[LENGTH_OF_STRING], CAR* carriers[2], int i, char clas[4], char dir, field_t** board, int* sizeYptr, int* sizeXptr, int newe_tab[4], char* newe_direction_ship, char digits[DIGITS_LENGTH], int pseudo_rem[2], int tab[4]) {
	defineTab(tab, carriers[player][i].fronY, carriers[player][i].frontX, carriers[player][i].backY, carriers[player][i].backX);
	hasEngineAndFreeMoves(carriers[player][i].has_engine, carriers[player][i].number_moves_in_given_turn, carriers[player][i].max_num_of_moves, i, clas, dir);
	newPosition(tab, 5, carriers[player][i].direction, dir, board, i, clas, newe_tab, *sizeYptr, *sizeXptr, newe_direction_ship);
	clearPreviousPosition(board, tab[0], tab[1], tab[2], tab[3], digits);
	placeOnBoardAlmost(board, newe_tab[0], newe_tab[1], newe_tab[2], newe_tab[3], player, 5, pseudo_rem, digits, sizeYptr, sizeXptr, 0, i, 0, clas, main_command, dir);
	carriers[player][i].direction = *newe_direction_ship;
	carriers[player][i].fronY = newe_tab[0];
	carriers[player][i].frontX = newe_tab[1];
	carriers[player][i].backY = newe_tab[2];
	carriers[player][i].backX = newe_tab[3];
	carriers[player][i].number_moves_in_given_turn += 1;
}



void movePlayerFrameBatt(int player, char main_command[LENGTH_OF_STRING], BAT* battleships[2], int i, char clas[4], char dir, field_t** board, int* sizeYptr, int* sizeXptr, int newe_tab[4], char* newe_direction_ship, char digits[DIGITS_LENGTH], int pseudo_rem[2], int tab[4]) {
	defineTab(tab, battleships[player][i].fronY, battleships[player][i].frontX, battleships[player][i].backY, battleships[player][i].backX);
	hasEngineAndFreeMoves(battleships[player][i].has_engine, battleships[player][i].number_moves_in_given_turn, battleships[player][i].max_num_of_moves, i, clas, dir);
	newPosition(tab, 4, battleships[player][i].direction, dir, board, i, clas, newe_tab, *sizeYptr, *sizeXptr, newe_direction_ship);
	clearPreviousPosition(board, tab[0], tab[1], tab[2], tab[3], digits);
	placeOnBoardAlmost(board, newe_tab[0], newe_tab[1], newe_tab[2], newe_tab[3], player, 4, pseudo_rem, digits, sizeYptr, sizeXptr, 0, i, 0, clas, main_command, dir);
	battleships[player][i].direction = *newe_direction_ship;
	battleships[player][i].fronY = newe_tab[0];
	battleships[player][i].frontX = newe_tab[1];
	battleships[player][i].backY = newe_tab[2];
	battleships[player][i].backX = newe_tab[3];
	battleships[player][i].number_moves_in_given_turn += 1;
}



void movePlayerFrameCrui(int player, char main_command[LENGTH_OF_STRING], CRU* cruisers[2], int i, char clas[4], char dir, field_t** board, int* sizeYptr, int* sizeXptr, int newe_tab[4], char* newe_direction_ship, char digits[DIGITS_LENGTH], int pseudo_rem[2], int tab[4]) {
	defineTab(tab, cruisers[player][i].fronY, cruisers[player][i].frontX, cruisers[player][i].backY, cruisers[player][i].backX);
	hasEngineAndFreeMoves(cruisers[player][i].has_engine, cruisers[player][i].number_moves_in_given_turn, cruisers[player][i].max_num_of_moves, i, clas, dir);
	newPosition(tab, 3, cruisers[player][i].direction, dir, board, i, clas, newe_tab, *sizeYptr, *sizeXptr, newe_direction_ship);
	clearPreviousPosition(board, tab[0], tab[1], tab[2], tab[3], digits);
	placeOnBoardAlmost(board, newe_tab[0], newe_tab[1], newe_tab[2], newe_tab[3], player, 3, pseudo_rem, digits, sizeYptr, sizeXptr, 0, i, 0, clas, main_command, dir);
	cruisers[player][i].direction = *newe_direction_ship;
	cruisers[player][i].fronY = newe_tab[0];
	cruisers[player][i].frontX = newe_tab[1];
	cruisers[player][i].backY = newe_tab[2];
	cruisers[player][i].backX = newe_tab[3];
	cruisers[player][i].number_moves_in_given_turn += 1;
}



void movePlayerFrameDest(int player, char main_command[LENGTH_OF_STRING], DES* destroyers[2], int i, char clas[4], char dir, field_t** board, int* sizeYptr, int* sizeXptr, int newe_tab[4], char* newe_direction_ship, char digits[DIGITS_LENGTH], int pseudo_rem[2], int tab[4]) {
	defineTab(tab, destroyers[player][i].fronY, destroyers[player][i].frontX, destroyers[player][i].backY, destroyers[player][i].backX);
	hasEngineAndFreeMoves(destroyers[player][i].has_engine, destroyers[player][i].number_moves_in_given_turn, destroyers[player][i].max_num_of_moves, i, clas, dir);
	newPosition(tab, 2, destroyers[player][i].direction, dir, board, i, clas, newe_tab, *sizeYptr, *sizeXptr, newe_direction_ship);
	clearPreviousPosition(board, tab[0], tab[1], tab[2], tab[3], digits);
	placeOnBoardAlmost(board, newe_tab[0], newe_tab[1], newe_tab[2], newe_tab[3], player, 2, pseudo_rem, digits, sizeYptr, sizeXptr, 0, i, 0, clas, main_command, dir);
	destroyers[player][i].direction = *newe_direction_ship;
	destroyers[player][i].fronY = newe_tab[0];
	destroyers[player][i].frontX = newe_tab[1];
	destroyers[player][i].backY = newe_tab[2];
	destroyers[player][i].backX = newe_tab[3];
	destroyers[player][i].number_moves_in_given_turn += 1;
}



void movePlayerFrame(int player, char main_command[LENGTH_OF_STRING], CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2], int i, char clas[4], char dir,field_t** board, int* sizeYptr, int* sizeXptr) {
	int newe_tab[4] = { 0,0,0,0 };
	char newe_direction_shipe = 'U';
	char* newe_direction_ship = &newe_direction_shipe;
	int tab[4] = { 0,0,0,0 };
	int pseudo_rem[2] = { 0,0 };
	char digits[DIGITS_LENGTH] = "00000";
	if (strcmp(clas, "CAR") == 0) {
		movePlayerFrameCarr(player, main_command, carriers, i, clas, dir, board, sizeYptr, sizeXptr, newe_tab, newe_direction_ship, digits, pseudo_rem, tab);
	}
	else if (strcmp(clas, "BAT") == 0) {
		movePlayerFrameBatt(player, main_command, battleships, i, clas, dir, board, sizeYptr, sizeXptr, newe_tab, newe_direction_ship, digits, pseudo_rem, tab);
	}
	else if (strcmp(clas, "CRU") == 0) {
		movePlayerFrameCrui(player, main_command, cruisers, i, clas, dir, board, sizeYptr, sizeXptr, newe_tab, newe_direction_ship, digits, pseudo_rem, tab);
	}
	else if (strcmp(clas, "DES") == 0) {
		movePlayerFrameDest(player, main_command, destroyers, i, clas, dir, board, sizeYptr, sizeXptr, newe_tab, newe_direction_ship, digits, pseudo_rem, tab);
	}
}



void movePlayer(char* nexteptr, char main_command[LENGTH_OF_STRING], field_t** board, CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2], int* sizeYptr, int* sizeXptr) {
	int i;
	int player = playerIndex(*nexteptr);
	char clas[4], dir;
	cin >> i >> clas >> dir;
	movePlayerFrame(player, main_command, carriers, battleships, cruisers, destroyers, i, clas, dir, board, sizeYptr, sizeXptr);
}



void tooManyShoots(int shoots_made, int max_shoots, int y, int x, int j, char clas[4]) {
	if (shoots_made >= max_shoots) {
		printf_s("INVALID OPERATION \"SHOOT %d %s %d %d\": TOO MANY SHOOTS", j, clas,y ,x);
		exit(1);
	}
}



void destroyedCanon(int has_canon, int y, int x, int j, char clas[4]) {
	if (!has_canon) {
		printf_s("INVALID OPERATION \"SHOOT %d %s %d %d\": SHIP CANNOT SHOOT", j, clas, y, x);
		exit(1);
	}
}



void canonPosition(int* head_y, int* head_x, field_t** board, int sizeY, int sizeX) { //calculting canon position for advanced shooting
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2;j++) {
			if (i == 0 && j == 0) {
				continue;
			}
			if (*head_y + i >= 0 && *head_y + i < sizeY && *head_x + j >= 0 && *head_x + j < sizeX  && board[*head_y + i][*head_x + j].content == '+') {
				*head_y = *head_y + i;
				*head_x = *head_x + j;
				break;
				break;
			}
		}
	}
}



void shootingTooFar(double range, int head_y, int head_x, int y, int x, int j, char clas[4], field_t** board, int sizeY, int sizeX) {
	int* canon_y_ptr = &head_y;
	int* canon_x_ptr = &head_x;
	canonPosition(canon_y_ptr, canon_x_ptr, board, sizeY, sizeX);
	double result = sqrt(pow(*canon_y_ptr - y, 2) + pow(*canon_x_ptr - x, 2));
	if (result > range) {
		printf_s("INVALID OPERATION \"SHOOT %d %s %d %d\": SHOOTING TOO FAR", j, clas, y, x);
		exit(1);
	}
}



void findShipAdvancedShootCarr(char clas[4], int i, int player, CAR* carriers[2], int y, int x) { //which ship is shooting
	destroyedCanon(carriers[player][i].has_canon, y, x, i, clas);
	tooManyShoots(carriers[player][i].number_shoots, 5, y, x, i, clas);
	carriers[player][i].number_shoots += 1;
}



void findShipAdvancedShootBatt(char clas[4], int i, int player, BAT* battleships[2],  int y, int x, field_t** board, int* sizeYptr, int* sizeXptr) { //which ship is shooting
	destroyedCanon(battleships[player][i].has_canon, y, x, i, clas);
	tooManyShoots(battleships[player][i].number_shoots, 4, y, x, i, clas);
	shootingTooFar(4, battleships[player][i].fronY, battleships[player][i].frontX, y, x, i, clas, board, *sizeYptr, *sizeXptr);
	battleships[player][i].number_shoots += 1;
}



void findShipAdvancedShootCrui(char clas[4], int i, int player, CRU* cruisers[2],  int y, int x, field_t** board, int* sizeYptr, int* sizeXptr) { //which ship is shooting
	destroyedCanon(cruisers[player][i].has_canon, y, x, i, clas);
	tooManyShoots(cruisers[player][i].number_shoots, 3, y, x, i, clas);
	shootingTooFar(3, cruisers[player][i].fronY, cruisers[player][i].frontX, y, x, i, clas, board, *sizeYptr, *sizeXptr);
	cruisers[player][i].number_shoots += 1;
}



void findShipAdvancedShootDest(char clas[4], int i, int player, DES* destroyers[2],  int y, int x, field_t** board, int* sizeYptr, int* sizeXptr) { //which ship is shooting
	destroyedCanon(destroyers[player][i].has_canon, y, x, i, clas);
	tooManyShoots(destroyers[player][i].number_shoots, 2, y, x, i, clas);
	shootingTooFar(2, destroyers[player][i].fronY, destroyers[player][i].frontX, y, x, i, clas, board, *sizeYptr, *sizeXptr);
	destroyers[player][i].number_shoots += 1;
}



void findShipAdvancedShoot(char clas[4], int i, int player, CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2],  int y, int x, int remainig_parts[2], field_t** board, int* sizeYptr, int* sizeXptr) {
	if (strcmp(clas, "CAR") == 0) {
		findShipAdvancedShootCarr(clas, i, player, carriers, y, x);
	}
	else if (strcmp(clas, "BAT") == 0) {
		findShipAdvancedShootBatt(clas, i, player, battleships, y, x, board, sizeYptr, sizeXptr);
	}
	else if (strcmp(clas, "CRU") == 0) {
		findShipAdvancedShootCrui(clas, i, player, cruisers, y, x, board, sizeYptr, sizeXptr);
	}
	else if (strcmp(clas, "DES") == 0) {
		findShipAdvancedShootDest(clas, i, player, destroyers, y, x, board, sizeYptr, sizeXptr);
	}
	if (shipWasHitAdvanced(board, y, x, carriers, battleships, cruisers, destroyers, sizeYptr, sizeXptr)) {// checking if we hit a ship
		decreaseRemainingParts(remainig_parts, board, y, x);
	}
}



void advancedShootFunct(field_t** board, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int ships_already_present[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], char move_player_command[LENGTH_OF_STRING], int* sizeYptr, int* sizeXptr, int remaining_parts[2],int player, CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2]) {
	int i, y, x;
	char clas[4];
	cin >> i >> clas >> y >> x;
	notAllShipsPresent(y, x, size_of_players, ships_already_present, move_player_command);
	beyondBoard(y, x, sizeYptr, sizeXptr, move_player_command);
	findShipAdvancedShoot(clas, i, player, carriers, battleships, cruisers, destroyers, y, x, remaining_parts, board, sizeYptr, sizeXptr);
}



void radaring(field_t** board, int range, int player, int sizeYptr, int sizeXptr, int radarY, int radarX) { //making fields visible for given player in range for given radar
	for (int i = -range;i <= range; i++) {
		for (int j = -range;j <=  range; j++) {
			if (radarY + i >= 0 && radarY + i < sizeYptr && radarX + j >= 0 && radarX + j < sizeXptr) {
				if (player == 0 && board[radarY + i][radarX + j].visible_to_A) {
					continue;
				}
				if (player == 1 && board[radarY + i][radarX + j].visible_to_B) {
					continue;
				}
				double rangeD = range;
				double result = sqrt(pow(i, 2) + pow(j, 2));
				if (result <= rangeD) {
					if (player == 0) {
						board[radarY + i][radarX + j].visible_to_A = 1;
					}
					else {
						board[radarY + i][radarX + j].visible_to_B = 1;
					}
				}
			}
		}
	}
}



void planeVisibility(field_t** board, int player, int sizeYptr, int sizeXptr, int y, int x) { //making some fields visible by SPY command
	for (int i = -1; i < 2;i++) {
		for (int j = -1; j < 2;j++) {
			if (y + i >= 0 && y + i < sizeYptr && x + j >= 0 && x + j < sizeXptr) {
				if (player == 0) {
					board[y+i][x+j].visible_to_A = 1;
				}
				else {
					board[y+i][x+j].visible_to_B = 1;
				}
			}
		}
	}
}



void checkWhichVisibleToPlayer(field_t** board, int sizeYptr, int sizeXptr) {
	for (int y = 0; y < sizeYptr; y++) {
		for (int x = 0; x < sizeXptr; x++) {
			if (board[y][x].is_head && board[y][x].content == '+') { //radaring for range of radar
				radaring(board, board[y][x].length_of_present_ship, board[y][x].occupied_by_B, sizeYptr, sizeXptr, y, x);
			}
			else if (board[y][x].is_head) {  //radaring for range 1 when radar destroyed
				radaring(board, 1, board[y][x].occupied_by_B, sizeYptr, sizeXptr, y, x);
			}
			if (board[y][x].occupied_by_A) {
				board[y][x].visible_to_A = 1;
			}
			if (board[y][x].occupied_by_B) {
				board[y][x].visible_to_B = 1;
			}
			if (board[y][x].is_plane_A) {
				planeVisibility(board, 0, sizeYptr, sizeXptr, y, x);
			}
			if (board[y][x].is_plane_B) {
				planeVisibility(board, 1, sizeYptr, sizeXptr, y, x);
			}
		}
	}
}



void clearView(field_t** board, int sizeYptr, int sizeXptr) { //clearing it after each move, in case of moving ships or radar destruction
	for (int y = 0; y < sizeYptr; y++) {
		for (int x = 0; x < sizeXptr; x++) {
			board[y][x].visible_to_A = 0;
			board[y][x].visible_to_B = 0;
		}
	}
}



void playerPrint(int player, field_t** board, int remainig_parts[2], int *sizeYptr, int *sizeXptr) {
	int modee;
	cin >> modee;
	checkWhichVisibleToPlayer(board, *sizeYptr, *sizeXptr);
	if (modee == 0) {
		printQuestion(board, player, *sizeYptr, *sizeXptr, remainig_parts, 0);
	}
	else if (modee == 1) {
		printOneQuestion(sizeYptr, sizeXptr, board, remainig_parts, player, 0);
	}
}



void shipHasnotCanon(CAR* carriers[2], int player, int i, int y, int x) {
	if (carriers[player][i].has_canon == 0) {
		printf_s("INVALID OPERATION \"SPY %d %d %d\": CANNOT SEND A PLANE", i, y, x);
		exit(1);
	}
}



void tooManyPlanes(CAR* carriers[2], int player, int i, int y, int x) {
	if (carriers[player][i].number_shoots >= carriers[player][i].lenght) {
		printf_s("INVALID OPERATION \"SPY %d %d %d\": ALL PLANES SENT", i, y, x);
		exit(1);
	}
}



void spyCommand(field_t** board, CAR* carriers[2], int player) {
	int i, y, x;
	cin >> i >> y >> x;
	shipHasnotCanon(carriers, player, i, y, x);
	tooManyPlanes(carriers, player, i, y, x);
	if (player == 0) {
		board[y][x].is_plane_A = 1;
	}
	else {
		board[y][x].is_plane_B = 1;
	}
	carriers[player][i].number_shoots += 1;
}



void clearPlanes(field_t** board, int sizeYptr, int sizeXptr) { //planes are only present for one turn
	for (int y = 0; y < sizeYptr; y++) {
		for (int x = 0; x < sizeXptr; x++) {
			board[y][x].is_plane_A = 0;
			board[y][x].is_plane_B = 0;
		}
	}
}



void playerMove(char* nexteptr,char main_command[LENGTH_OF_STRING], field_t** board, int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2], int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int ships_already_present[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], int* sizeYptr, int* sizeXptr, int remainig_parts[2], int* is_game_extended) {
	char move_player_command[LENGTH_OF_STRING] = "sth";
	int player = playerIndex(*nexteptr);
	while (strcmp(main_command, move_player_command) != 0) {
		cin >> move_player_command;
		if (strcmp(move_player_command, "PLACE_SHIP") == 0) {
			placeShip(player,move_player_command, board, size_of_players, carriers, battleships, cruisers, destroyers, init_positions, ships_already_present, remainig_parts, sizeYptr, sizeXptr);
		}
		else if (*is_game_extended == 0 && strcmp(move_player_command, "SHOOT") == 0) {
			shootFunct(board, size_of_players, ships_already_present, move_player_command, sizeYptr, sizeXptr, remainig_parts, carriers, battleships, cruisers, destroyers);
		}
		else if (strcmp(move_player_command, "MOVE") == 0) {
			movePlayer(nexteptr, move_player_command, board, carriers, battleships, cruisers, destroyers, sizeYptr, sizeXptr);
		}
		else if (*is_game_extended == 1 && strcmp(move_player_command, "SHOOT") == 0) {
			advancedShootFunct(board, size_of_players, ships_already_present, move_player_command, sizeYptr, sizeXptr, remainig_parts, player, carriers, battleships, cruisers, destroyers);
		}
		else if (strcmp(move_player_command, "PRINT") == 0) {
			playerPrint(player, board, remainig_parts, sizeYptr, sizeXptr);
		}
		else if (strcmp(move_player_command, "SPY") == 0) {
			spyCommand(board, carriers, player);
		}
		clearView(board, *sizeYptr, *sizeXptr);
	}
	clearPlanes(board, *sizeYptr, *sizeXptr);
}



void clearShootsAndMoves(int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES], CAR* carriers[2], BAT* battleships[2], CRU* cruisers[2], DES* destroyers[2]) {
	int iterations[4];
	for (int i = 0; i < 4; i++) {
		if (size_of_players[0][i] >= size_of_players[1][i]) {
			iterations[i] = size_of_players[0][i];
		}
		else {
			iterations[i] = size_of_players[1][i];
		}
	}
	for (int i = 0; i < iterations[0]; i++) {
		carriers[0][i].number_shoots = 0; carriers[1][i].number_shoots = 0; carriers[0][i].number_moves_in_given_turn = 0; carriers[1][i].number_moves_in_given_turn = 0;
	}
	for (int i = 0; i < iterations[1]; i++) {
		battleships[0][i].number_shoots = 0; battleships[1][i].number_shoots = 0; battleships[0][i].number_moves_in_given_turn = 0; battleships[1][i].number_moves_in_given_turn = 0;
	}
	for (int i = 0; i < iterations[2]; i++) {
		cruisers[0][i].number_shoots = 0; cruisers[1][i].number_shoots = 0; cruisers[0][i].number_moves_in_given_turn = 0; cruisers[1][i].number_moves_in_given_turn = 0;
	}
	for (int i = 0; i < iterations[3]; i++) {
		destroyers[0][i].number_shoots = 0; destroyers[1][i].number_shoots = 0; destroyers[0][i].number_moves_in_given_turn = 0; destroyers[1][i].number_moves_in_given_turn = 0;
	}
}



int main() {
	CAR* carriers[2];
	BAT* battleships[2];
	CRU* cruisers[2];
	DES* destroyers[2];

	char nexte_player = 'A';
	char* nextePtr = &nexte_player;

	int sizeY = 21;
	int* sizeYptr = &sizeY;
	int sizeX = 10;
	int* sizeXptr = &sizeX;

	field_t** board;

	int is_game_extended = 0;
	int* extended_ptr = &is_game_extended;

	int remainig_parts[2] = { 0,0 };
	int ships_already_present[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES] = { {0,0,0,0},{0,0,0,0} };
	int init_positions[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES] = { {0,0,9,9},{11,0,20,9} }; // player1 y1,x1,y2,x2 player2 y1,x1,y2,x2
	int size_of_players[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_CLASSES] = { { 2,2,3,4 }, { 2,2,3,4 } };

	board = createMap(sizeYptr, sizeXptr); // board

	char main_command[LENGTH_OF_STRING];

	defineFleet(carriers, battleships, cruisers, destroyers, size_of_players);

	while (cin >> main_command) {
		if (strcmp(main_command, "[state]") == 0) {
			board = state(sizeYptr, sizeXptr, nextePtr, board,size_of_players, carriers, battleships, cruisers, destroyers, init_positions, remainig_parts, ships_already_present, extended_ptr);
		}
		if (strcmp(main_command, "[playerA]") == 0) {
			if (nexte_player == 'B') {
				printf_s("INVALID OPERATION \"[playerA] \": THE OTHER PLAYER EXPECTED");
				exit(1);
			}
			playerMove(nextePtr,main_command, board, size_of_players, carriers, battleships, cruisers, destroyers, init_positions, ships_already_present, sizeYptr, sizeXptr, remainig_parts, extended_ptr);
			nexte_player = 'B';
		}
		else if (strcmp(main_command, "[playerB]") == 0) {
			if (nexte_player == 'A') {
				printf_s("INVALID OPERATION \"[playerB] \": THE OTHER PLAYER EXPECTED");
				exit(1);
			}
			playerMove(nextePtr,main_command, board, size_of_players, carriers, battleships, cruisers, destroyers, init_positions, ships_already_present, sizeYptr, sizeXptr, remainig_parts, extended_ptr);
			nexte_player = 'A';
		}
		clearShootsAndMoves(size_of_players, carriers, battleships, cruisers, destroyers);
	}
	for (int i = 0; i < 2; i++) {
		delete carriers[i];
		delete battleships[i];
		delete cruisers[i];
		delete destroyers[i];
	}
	
	for (int i = 0; i < *sizeYptr; i++) {
		delete[] board[i];
	}
	delete[] board;
	return 0;
}