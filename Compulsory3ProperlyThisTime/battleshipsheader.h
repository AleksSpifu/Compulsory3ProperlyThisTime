#pragma once

const int M = 6, N = 6;
const char BLANK = ' ';
const char SHIP = 'S';
const char MISS = 'M';
const char HIT = 'H';
const int numberOfShips = 6;
char board[M][N]; // 2d array with M rows and N columns
int numberOfHits;
int numberOfShots;


int letterToNumber(char letter);
void writeLetters();
void printPlayerBoard();
void printBoard();
void makeBoard();
void makeEmptyBoard();
int randomRow();
int randomColumn();