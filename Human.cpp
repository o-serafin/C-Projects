#include "Human.h"
#include <conio.h>
#include <cwchar>
#include <iostream>
#include <windows.h>


#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define SPECIAL_ABILITY '1'
#define SAVE 's'
#define LOAD 'l'

void Human::weaken() { 
    this->turns_remaining -= 1;
    if (this->strength > 5) {
        strength -= 1;
    }
}

void Human::action(int sizeY, int sizeX, int move[8], Creature*** board) {
    int c = _getwch();
    switch (c) {
    case KEY_UP:
        move[0] = -1;//key up
        break;
    case KEY_DOWN:
        move[0] = 1;   // key down
        break;
    case SAVE:
        move[0] = 8888;
        return;
        break;
    case LOAD:
        move[0] = 9999;
        return;
        break;
    case KEY_LEFT:
        move[1] = -1;  // key left
        break;
    case KEY_RIGHT:
        move[1] = 1;  // key right
        break;
    case SPECIAL_ABILITY:
        if (this->turns_remaining <= 0) {
            this->strength += 10;
            this->turns_remaining = 15;
            std::cout << "Human activated special ability" << std::endl;
        }
        else {
            std::cout << "Unable to turn on special ability for " << this->turns_remaining << " turns" << std::endl;
        }
        
    default:
        action(sizeX, sizeY, move, board);
        break;
    }
    if (this->y + move[0] < 0 || this->y + move[0] >= sizeY || this->x + move[1] < 0 || this->x + move[1] >= sizeX) {
        move[0] = 0;
        move[1] = 0;
    }
    return;
}
