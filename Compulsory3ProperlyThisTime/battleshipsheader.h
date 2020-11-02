#pragma once

//This is what came from the assignement. I made number of ships scale with the board, and in the code i made number of shots scale with ships.
//So everything updates from the size of the board.
const int M = 6, N = 6;
const char BLANK = ' ';
const char SHIP = 'S';
const char MISS = 'M';
const char HIT = 'H';
const int numberOfShips = (M + N);
char playerBoard[M][N]; // 2d array with M rows and N columns
int numberOfHits{};
int numberOfShots{};

int numberOfShotsPerShip = 4;

//Then declare the AI stuff.
char aiBoard[M][N];
int aiNumberOfHits{};
int aiNumberOfShots{};

//This is what keeps all the values of all the tiles for the AI.
std::vector <std::vector <int> > aiTargetValues(M, std::vector <int> (N, 1));

//Here you can toggle from the "Cheating" mode where you can see the enemies ships, and turn on and off the three-tiled ships, so they spread out more.
bool displayShipLocations = false;
bool threeTiledShips = true;

//Then i declare all the battleships functions. 
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