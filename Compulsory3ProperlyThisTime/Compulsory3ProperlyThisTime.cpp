#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "comp3header.h"
#include "battleshipsheader.h"


int main()
{
    srand(time(NULL));
    makeEmptyBoard();
    makeBoard();
    printBoard();

    writeLetters();
    /*int loginValue = login();
    if (loginValue == 2) {
        std::cout << "Error. The gridsize exceeds 26 characters and can therefore not fit the alphabet. Please change the values.";
    }
    while (loginValue == 0) {
        loginValue = login();
    }
    if (loginValue == 1) {
        std::cout << "Successful login!\n";
        system("pause");
        mainMenu();
    }*/

    return 0;
}

void makeEmptyBoard()
{
    for (int r = 0; r < M; r++) {
        for (int c = 0; c < N; c++) {
            board[r][c] = BLANK;
        }
    }
}

void makeBoard()
{
    int shipsLeft = numberOfShips;
    while (shipsLeft > 0) {
        int row = randomRow();
        int col = randomColumn();
        if (board[row][col] != SHIP) {
            board[row][col] = SHIP;
            shipsLeft--;
        }
    }
}

void printBoard()
{
    for (int r = 0; r < M; r++) {
        if ((M - r) >= 10) {
            std::cout << M - r;
        }
        else {
            std::cout << M - r << " ";
        }
        for (int c = 0; c < N; c++) {
            if (c > 0) {
                std::cout << " ";
            }
            std::cout << "| " << board[r][c];
        }
        std::cout << " |\n";
    }
}

void printPlayerBoard()
{
    for (int r = 0; r < M; r++) {
        if ((M - r) >= 10) {
            std::cout << M - r;
        }
        else {
            std::cout << M - r << " ";
        }
        for (int c = 0; c < N; c++) {
            if (c > 0) {
                std::cout << " ";
            }
            char output = board[r][c];
            if (output == SHIP) {
                output = BLANK;
            }
            std::cout << "| " << output;
        }
        std::cout << " |\n";
    }
}

void writeLetters()
{
    std::cout << "    ";
    for (int i = 0; i < N; i++) {
        char output = i + 65;
        std::cout << output << "   ";
    }
    std::cout << std::endl;
}

int randomRow()
{
    int temp = rand() % M;
    return temp;
}

int randomColumn()
{
    int temp = rand() % N;
    return temp;
}

int letterToNumber(char letter) // only big letters
{
    return static_cast<int>(letter) - 65;
}

void mainMenu()
{

    bool userWantsToExit = false;
    while (!userWantsToExit) {
        system("cls");
        std::cout << "Main menu.\n";
        std::cout << "1. Change the password.\n2. Play Battleship.\n3. Quit.\n";
        switch (tolower(_getch()))
        {
        case '1':
            changePassword();
            break;
        case '2':
            //battleships
            break;
        case '3':
            userWantsToExit = true;
            break;
        default:
            break;
        }
    }

}

void changePassword()
{
    //std::ofstream passwordFile(fileName);
    std::vector <int> oldPassword{};
    oldPassword = readPassFromFile();
    std::vector <int> oldPasswordInput{};
    bool confirmedOldPass = true;
    do {
        confirmedOldPass = true;
        oldPasswordInput = typingInPassword(loginScreen(), loggingIn);
        if (oldPassword.size() == oldPasswordInput.size()) {
            for (int i = 0; i < oldPassword.size(); i++) {
                if (oldPassword[i] != oldPasswordInput[i]) {
                    confirmedOldPass = false;
                }
            }
        }
        else {
            confirmedOldPass = false;
        }
        if (!confirmedOldPass) {

            std::cout << "Incorrect. Please try again.\n";
            system("pause");
        }
    } while (!confirmedOldPass);

    bool newPassAccepted = true;
    std::vector <int> newPass1{};
    std::vector <int> newPass2{};
    do {
        newPassAccepted = true;
        newPass1 = typingInPassword(loginScreen(), makeNewPassword);
        newPass2 = typingInPassword(loginScreen(), confirmPassword);
        if (newPass1.size() == newPass2.size()) {
            for (int i = 0; i < newPass1.size(); i++) {
                if (newPass1[i] != newPass2[i]) {
                    newPassAccepted = false;
                }
            }
        }
        else {
            confirmedOldPass = false;
        }
        if (!newPassAccepted) {
            std::cout << "Passwords do not match. Please try again.\n";
            system("pause");

        }
        else {
            std::ofstream passwordFile;
            passwordFile.open(fileName);
            std::string output{};
            for (int i = 0; i < newPass1.size(); i++) {
                char outCh = newPass1[i] + 64;
                output.push_back(outCh);
            }
            passwordFile << output;
            std::cout << "Password Change successful!\n";
            system("pause");
            passwordFile.close();
        }
    } while (!newPassAccepted);

}

std::vector <std::vector <int>> loginScreen() {
    std::vector <std::vector <int>> screen(loginGridSizeY, std::vector <int>(loginGridSizeX, 0));
    for (int r = 0; r < screen.size(); r++) {
        for (int c = 0; c < screen[r].size(); c++) {
            screen[r][c] = (r * screen[r].size()) + (c + 1);
        }
    }
    return screen;
}

int login() {

    if (loginGridSizeX * loginGridSizeY > 26) {
        return 2;
    }


    std::vector <int> inputFromFile = {};
    inputFromFile = readPassFromFile();

    if (inputFromFile.size() < minimumPasswordSize) {
        makeFirstPassword(loginScreen());
    }

    inputFromFile = readPassFromFile();
    bool passwordsMatch = true;
    system("cls");

    std::vector <int> passHolder = typingInPassword(loginScreen(), loggingIn);

    if (passHolder.size() == inputFromFile.size()) {
        for (int i = 0; i < passHolder.size(); i++) {
            if (passHolder[i] != inputFromFile[i]) {
                passwordsMatch = false;
            }
        }
    }
    else {
        passwordsMatch = false;
    }


    if (passwordsMatch) {
        return 1;
    }
    else {
        std::cout << "Wrong password. Please try again.\n";
        system("pause");
        return 0;

    }

}

std::vector <int> readPassFromFile() {
    std::ifstream passFile;
    std::string rawInput = "";
    std::vector <int> output = {};
    passFile.open(fileName);
    std::getline(passFile, rawInput);
    passFile.close();

    for (int i = 0; i < rawInput.size(); i++) {
        output.push_back(rawInput[i] - 64);
    }

    return output;
}

void makeFirstPassword(std::vector <std::vector <int>> screen) {
    bool acceptedPassword = false;
    while (!acceptedPassword) {
        std::ofstream passwordFile;
        std::vector <int> passInput = {};
        std::vector <int> comparePassword = {};
        std::string passOutput = "";
        passInput = typingInPassword(screen, makeInitialPassword);

        passwordFile.open(fileName);
        for (int i = 0; i < passInput.size(); i++) {
            char tempChar = passInput[i] + 64;
            passOutput.push_back(tempChar);
        }

        passwordFile << passOutput;
        passwordFile.close();

        passInput.clear();
        passOutput.clear();

        passInput = typingInPassword(screen, confirmPassword);

        bool passwordsMatch = true;

        comparePassword = readPassFromFile();



        acceptedPassword = true;
        if (comparePassword.size() == passInput.size()) {
            for (int i = 0; i < comparePassword.size(); i++) {
                if (comparePassword[i] != passInput[i]) {
                    passwordsMatch = false;
                }
            }
        }

        else {
            std::cout << "Passwords don't match. Please try again.\n";
            passwordFile.open(fileName);
            passwordFile << ".";
            passwordFile.close();
            acceptedPassword = false;
        }
        if (!passwordsMatch) {
            std::cout << "Passwords don't match. Please try again.\n";
            passwordFile.open(fileName);
            passwordFile << ".";
            passwordFile.close();
            acceptedPassword = false;
        }
        else {
            std::cout << "Success!\n";
        }



        system("pause");
    }


}

std::vector <int> typingInPassword(std::vector <std::vector <int>> screen, int scenario) {
    std::vector <int> passInput;
    int moveDirection[2] = { 0, 0 };
    int cursor[2] = { 0, 0 };
    bool doneTyping = false;



    while (!doneTyping) {
        if (passInput.size() == 0) {
            passInput.push_back(screen[cursor[0]][cursor[0]]);
        }
        system("cls");
        switch (scenario)
        {
        case makeInitialPassword:
            std::cout << "Looks like you haven't got a password yet. Let's set one! Minimum " << minimumPasswordSize << " characters, including your starting position.\n";
            std::cout << "Please enter your new password.\n";
            if (passInput.size() < minimumPasswordSize) {
                std::cout << "You need at least " << minimumPasswordSize - passInput.size() << " more characters.\n";
            }
            else {
                std::cout << "You have reached the minimum amount of characters. Feel free to keep adding more.\n";
            }
            break;
        case confirmPassword:
            std::cout << "Now please confirm your password.\n";
            break;
        case loggingIn:
            std::cout << "Please enter your current password to log in.\n";
            break;
        case makeNewPassword:
            std::cout << "Please enter your new password.\n";
            break;
        default:
            break;
        }
        std::cout << "Press WASD to move, and Enter to finish.\n";
        for (int i = 0; i <= loginGridSizeX; i++) {
            std::cout << "----";
        }
        std::cout << "\n";
        for (int r = 0; r < screen.size(); r++) {
            for (int c = 0; c < screen[r].size(); c++) {
                if (r == cursor[0] && c == cursor[1]) {
                    std::cout << " | *";
                }
                else {
                    char output = screen[r][c] + 64;
                    std::cout << " | " << output;
                }

            }
            std::cout << " | \n";
            for (int i = 0; i <= loginGridSizeX; i++) {
                std::cout << "----";
            }
            std::cout << "\n";
        }
        switch (tolower(_getch()))
        {
        case 'w':
            moveDirection[0] = -1;
            break;
        case 's':
            moveDirection[0] = 1;
            break;
        case 'a':
            moveDirection[1] = -1;
            break;
        case 'd':
            moveDirection[1] = 1;
            break;
        case '\r':
            if (passInput.size() >= minimumPasswordSize) {
                doneTyping = true;
            }
            else {
                std::cout << "Password too short. Please try again.\n";
                passInput.clear();
                cursor[0] = 0;
                cursor[1] = 0;
                system("pause");
            }

            break;
        default:
            break;
        }

        if (!doneTyping && (moveDirection[0] != 0 || moveDirection[1] != 0)) {
            if ((cursor[0] + moveDirection[0] < screen.size()) && (cursor[0] + moveDirection[0] >= 0) && (cursor[1] + moveDirection[1] < screen[0].size()) && (cursor[1] + moveDirection[1] >= 0)) {
                cursor[0] += moveDirection[0];
                cursor[1] += moveDirection[1];
                moveDirection[0] = 0;
                moveDirection[1] = 0;
                passInput.push_back(screen[cursor[0]][cursor[1]]);
            }
            else {
                moveDirection[0] = 0;
                moveDirection[1] = 0;
            }
        }
    }

    return passInput;
}