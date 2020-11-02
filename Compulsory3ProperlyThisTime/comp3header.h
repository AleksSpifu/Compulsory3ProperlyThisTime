#pragma once

//This header file handles all the login stuff, and these are the "Settings".
//If you change the grid size after a password has been set, there is no failsafe to make sure
//the password still works, so you might need to delete the password from the file.
const std::string fileName = "password.txt";
const int loginGridSizeX = 5;
const int loginGridSizeY = 5;
const int minimumPasswordSize = 5;

//These are to let the typingInPassword function know what to write to the user.
const int makeInitialPassword = 1;
const int confirmPassword = 2;
const int loggingIn = 3;
const int makeNewPassword = 4;

//This just declares all the login functions.
int login();
void makeFirstPassword(std::vector <std::vector <int>> screen);
std::vector <int> typingInPassword(std::vector <std::vector <int>> screen, int scenario);
std::vector <int> readPassFromFile();
void mainMenu();
void changePassword();
std::vector <std::vector <int>> loginScreen();