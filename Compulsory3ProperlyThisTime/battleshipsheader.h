#pragma once

const int M = 6, N = 6;
const char BLANK = ' ';
const char SHIP = 'S';
const char MISS = 'M';
const char HIT = 'H';
const int numberOfShips = (M + N);
char playerBoard[M][N]; // 2d array with M rows and N columns
int numberOfHits{};
int numberOfShots{};

char aiBoard[M][N];
int aiNumberOfHits{};
int aiNumberOfShots{};
std::vector <std::vector <int> > aiTargetValues(M, std::vector <int> (N, 1));

bool displayShipLocations = false;
bool threeTiledShips = true;

int letterToNumber(char letter);
void writeLetters();
void printPlayerBoard(char board[M][N]);
void printBoard(char board[M][N]);
void makeBoard(char board[M][N]);
void makeEmptyBoard(char board[M][N]);
int randomRow();
int randomColumn();
void play();
void shoot(char board[M][N]);
int getInputFromUser();
void makeBoard3(char board[M][N]);
void aiTask();