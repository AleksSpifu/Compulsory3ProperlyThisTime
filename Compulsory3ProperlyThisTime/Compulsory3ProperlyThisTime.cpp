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
    
    int loginValue = login();
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
    }

    return 0;
}

void play()
{
    makeEmptyBoard(playerBoard);
    makeEmptyBoard(aiBoard);
    if (threeTiledShips) {
        makeBoard3(playerBoard);
        makeBoard3(aiBoard);
    }
    else {
        makeBoard(playerBoard);
        makeBoard(aiBoard);
    }
    
    numberOfShots = numberOfShips * 4;
    aiNumberOfShots = numberOfShots;
    bool gameRunning = true;
    while (gameRunning) {
        std::cout << "You have " << numberOfShots << " shots left, there are " << numberOfShips - numberOfHits << " ships left, so far you have sunk " << numberOfHits << " ships.\n";
        std::cout << "The top board is where you will guess. The bottom one is your ships, and the AI will guess.\n";
        if (displayShipLocations) {
            printBoard(playerBoard);
        }
        else {
            printPlayerBoard(playerBoard);
        }
        
        writeLetters();
        std::cout << "\nYour ships:\n";
        std::cout << "Ai has " << aiNumberOfHits << " hits, " << aiNumberOfShots << " shots left, and has " << numberOfShips - aiNumberOfHits << " ships left to hit.\n";
        printBoard(aiBoard);
        shoot(playerBoard);
        aiTask();
        

        if (numberOfShots == 0 || aiNumberOfShots == 0) {
            gameRunning = false;
        }
        else if (numberOfHits == numberOfShips || aiNumberOfHits == numberOfShips) {
            gameRunning = false;
        }
        else {
            system("cls");
        }

    }
    system("cls");
    if (displayShipLocations) {
        printBoard(playerBoard);
    }
    else {
        printPlayerBoard(playerBoard);
    }

    writeLetters();
    std::cout << "\nYour ships:\n";
    std::cout << "Ai has " << aiNumberOfHits << " hits, " << aiNumberOfShots << " shots left, and has " << numberOfShips - aiNumberOfHits << " ships left to hit.\n";
    printBoard(aiBoard);
    if (aiNumberOfHits > numberOfHits) {
        std::cout << "Better luck next time!\n";
    }
    else if (numberOfHits > aiNumberOfHits) {
        std::cout << "Congratulations! You won!\n";
    }
    else if (numberOfHits == aiNumberOfHits) {
        std::cout << "It was a draw!\n";
    }
    system("pause");
    
}

void aiTask() 
{
    int targetRow{};
    int targetCol{};
    int highestValue{};
    for (int r = 0; r < M; r++) {
        for (int c = 0; c < M; c++) {
            if (aiTargetValues[r][c] > highestValue) {
                targetRow = r;
                targetCol = c;
                highestValue = aiTargetValues[r][c];
            }
        }
    }
    if (highestValue == 1) {
        bool notRepeatingShot = false;
        while (!notRepeatingShot) {
            targetRow = randomRow();
            targetCol = randomColumn();

            if (aiBoard[targetRow][targetCol] != HIT && aiBoard[targetRow][targetCol] != MISS) {
                notRepeatingShot = true;
            }
        }
        
    }
    
    if (aiBoard[targetRow][targetCol] == SHIP) {
        if (targetRow == 0) {
            if (targetCol == 0) {
                aiTargetValues[targetRow + 1][targetCol] *= 5;
                aiTargetValues[targetRow][targetCol + 1] *= 5;
            }
            else if (targetCol == (N-1)) {
                aiTargetValues[targetRow + 1][targetCol] *= 5;
                aiTargetValues[targetRow][targetCol - 1] *= 5;
            }
            else {
                aiTargetValues[targetRow][targetCol + 1] *= 5;
                aiTargetValues[targetRow][targetCol - 1] *= 5;
                aiTargetValues[targetRow + 1][targetCol] *= 5;
            }
        }
        else if (targetRow == (M - 1)) {
            if (targetCol == 0) {
                aiTargetValues[targetRow - 1][targetCol] *= 5;
                aiTargetValues[targetRow][targetCol + 1] *= 5;
            }
            else if (targetCol == (N - 1)) {
                aiTargetValues[targetRow - 1][targetCol] *= 5;
                aiTargetValues[targetRow][targetCol - 1] *= 5;
            }
            else {
                aiTargetValues[targetRow - 1][targetCol] *= 5;
                aiTargetValues[targetRow][targetCol - 1] *= 5;
                aiTargetValues[targetRow][targetCol + 1] *= 5;
            }
        }
        else {
            if (targetCol == 0) {
                aiTargetValues[targetRow - 1][targetCol] *= 5;
                aiTargetValues[targetRow + 1][targetCol] *= 5;
                aiTargetValues[targetRow][targetCol + 1] *= 5;
            }
            else if (targetCol == (N - 1)) {
                aiTargetValues[targetRow - 1][targetCol] *= 5;
                aiTargetValues[targetRow + 1][targetCol] *= 5;
                aiTargetValues[targetRow][targetCol - 1] *= 5;
            }
            else {
                aiTargetValues[targetRow - 1][targetCol] *= 5;
                aiTargetValues[targetRow + 1][targetCol] *= 5;
                aiTargetValues[targetRow][targetCol - 1] *= 5;
                aiTargetValues[targetRow][targetCol + 1] *= 5;
            }
        }       
        
        aiBoard[targetRow][targetCol] = HIT;
        aiNumberOfShots--;
        aiNumberOfHits++;
        aiTargetValues[targetRow][targetCol] *= 0;
    }
    else {
        aiBoard[targetRow][targetCol] = MISS;
        aiNumberOfShots--;
        aiTargetValues[targetRow][targetCol] *= 0;
    }
    

}

void makeBoard3(char board[M][N])
{
    int shipsLeft = numberOfShips;
    bool insertOnRow = true;
    while (shipsLeft >= 3) {
        int row = randomRow();
        int col = randomColumn();

        if (insertOnRow) {
            if (row > 0 && row < (M-1)) {
                if (board[row - 1][col] != SHIP && board[row][col] != SHIP && board[row + 1][col] != SHIP) {
                    board[row - 1][col] = SHIP;
                    board[row][col] = SHIP;
                    board[row + 1][col] = SHIP;
                    shipsLeft -= 3;
                    insertOnRow = false;
                }
            }
        }
        else if (!insertOnRow) {
            if (col > 0 && col < (N-1)) {
                if (board[row][col - 1] != SHIP && board[row][col] != SHIP && board[row][col + 1] != SHIP) {
                    board[row][col - 1] = SHIP;
                    board[row][col] = SHIP;
                    board[row][col + 1] = SHIP;
                    shipsLeft -= 3;
                    insertOnRow = true;
                }
            }
        }
    }

    //in case the number of ships doesn't work with the 3 long ships, it will make sure it gets to the correct amount with single ships. 
    while (shipsLeft > 0) {
        int row = randomRow();
        int col = randomColumn();
        if (board[row][col] != SHIP) {
            board[row][col] = SHIP;
            shipsLeft--;
        }
    }
}

void shoot(char board[M][N])
{
    std::cout << "Which tile would you like to hit?\n";
    int tempInt = getInputFromUser();
    int row{};
    int col{};

    while (tempInt >= 100) {
        col++;
        tempInt -= 100;
    }
    row = tempInt;

    if (board[row][col] == SHIP) {
        board[row][col] = HIT;
        numberOfHits++;
        numberOfShots--;
    }
    else if (board[row][col] == BLANK) {
        board[row][col] = MISS;
        numberOfShots--;
    }
    else {
        numberOfShots--;
    }

}

int getInputFromUser() {
    std::cout << "\nPlease enter the tile you would like to shoot. (A1 for example)\n";
    std::string input{};
    int returnValue = 0;
    bool acceptedInput = false;
    while (!acceptedInput) {
        std::cin >> input;
        if (input.size() == 2) {
            if ((input[0] >= 65 && input[0] <= 64 + N) || (input[0] >= 97 && input[0] <= 96 + N)) {
                if (input[1] > 48 && input[1] <= M + 48) {
                    returnValue += M-(input[1] - 48);
                    if (input[0] > 90) {
                        input[0] -= 32;
                    }
                    returnValue += (letterToNumber(input[0]) * 100);
                    return returnValue;
                    acceptedInput = true;
                }
            }
        }
        else if (input.size() > 2) {
            if ((input[0] >= 65 && input[0] <= 64 + N) || (input[0] >= 97 && input[0] <= 96 + N)) {
                std::string tempString{};
                int tempInt{};
                for (int i = 1; i < input.size(); i++) {
                    tempString.push_back(input[i]);
                }
                tempInt = (std::stoi(tempString));
                if (tempInt > 0 && tempInt <= M) {
                    returnValue += M-(tempInt);
                    if (input[0] > 90) {
                        input[0] -= 32;
                    }
                    
                    returnValue += (letterToNumber(input[0]) * 100);
                    
                    return returnValue;
                    acceptedInput = true;
                }
            }
        }
        std::cout << "Invalid input. Please try again.\n";
    }


}

void makeEmptyBoard(char board[M][N])
{
    for (int r = 0; r < M; r++) {
        for (int c = 0; c < N; c++) {
            board[r][c] = BLANK;
        }
    }
}

void makeBoard(char board[M][N])
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

void printBoard(char board[M][N])
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

void printPlayerBoard(char board[M][N])
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
            system("cls");
            play();
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