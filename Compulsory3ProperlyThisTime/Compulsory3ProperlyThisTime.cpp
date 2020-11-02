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
    //Either get an error from the login function, keep asking to log in, or go to menu.
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

//This is the main function for battleships. It just runs everything in the proper order, and continues until the game is over. 
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
    //Make sure the number of shots scales with the number of ships
    numberOfShots = numberOfShips * numberOfShotsPerShip;
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

//This function tells the AI where to shoot. If there hasn't been a hit yet, it will just shoot randomly. 
//If there has been a hit, every tile around it where a ship might be will get a higher value, and the AI will prioritize those.
void aiTask() 
{
    int targetRow{};
    int targetCol{};
    int highestValue{};

    //Get the row and column of the highest score, as well as the score.
    for (int r = 0; r < M; r++) {
        for (int c = 0; c < M; c++) {
            if (aiTargetValues[r][c] > highestValue) {
                targetRow = r;
                targetCol = c;
                highestValue = aiTargetValues[r][c];
            }
        }
    }

    //All the scores start at 1, so if this is the highest, just shoot wherever.
    if (highestValue == 1) {
        bool notRepeatingShot = false;
        while (!notRepeatingShot) {
            targetRow = randomRow();
            targetCol = randomColumn();

            //Make sure it's not shooting on a ship it has already sunk.
            if (aiBoard[targetRow][targetCol] != HIT && aiBoard[targetRow][targetCol] != MISS) {
                notRepeatingShot = true;
            }
        }
        
    }
    
    //if it hits, it will update the tiles around. This one is a giant mess, but i wanted to include the edges without
    //going beyond the borders of the array, so it's just a bunch of failsafes to make sure that doesn't happen. 
    //Otherwise, it will times it by 5. This way, if a hit has happened nearby, and therefore had it's value dropped to 0,
    //The value will not increase. If the value is the default, 1, it will be 5, and will be shot at next.
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
        
        //Then update the board, shots and hits etc.
        //and finally set the value of the tile to 0 as i explained above.
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

//this places ships of 3 tiles on the board.
void makeBoard3(char board[M][N])
{
    int shipsLeft = numberOfShips;

    //InsertOnRow will alternate every time a ship is placed, so half will be horizontal and half will be vertical.
    bool insertOnRow = true;
    while (shipsLeft >= 3) {
        int row = randomRow();
        int col = randomColumn();

        //Then it checks if a ship already exists on the space it chose. if it's clear, it'll put a ship there.
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

        //Same thing but horisontally.
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

//This is the shooting function. It converts the returned integer from getInputFromUser into a row and column, and tries to fire the shot. Then updates the required variables.
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

//This one looks horrible, but it's pretty simple. It asks for input, and then converts it to a row and a column, and returns it in a 3 or 4 digit "code". for example 204 means row 4 column 2.
//The reason it looks so bad is the conversion from a letter to a column, which i got working even with lowercase letters.
//but yeah. It seperates the letter from the number, if the number is more than 1 digit, it converts it to a string, then to an integer, and multiplies it with 100.
//then both are added together to return to the shoot function. Because i multiply by 100, this means i can never have more than 100 rows, but i don't think that'll ever happen.
int getInputFromUser() {
    std::cout << "\nPlease enter the tile you would like to shoot. (A1 for example)\n";
    std::string input{};
    int returnValue = 0;
    bool acceptedInput = false;
    while (!acceptedInput) {

        //Get a string from user.
        std::cin >> input;

        //if it only has 2 digits, easy peasy.
        if (input.size() == 2) {
            //Just make sure that the letter is actually on the board.
            if ((input[0] >= 65 && input[0] <= 64 + N) || (input[0] >= 97 && input[0] <= 96 + N)) {
                //Then make sure the number is on the board
                if (input[1] > 48 && input[1] <= M + 48) {
                    //Then add the row directly to returnValue
                    returnValue += M-(input[1] - 48);
                    //if the value is higher than 90, it's a lowercase character, and gets converted to an uppercase character.
                    if (input[0] > 90) {
                        input[0] -= 32;
                    }
                    //Then it gets converted to integer, multiplied by 100 and added to returnvalue.
                    returnValue += (letterToNumber(input[0]) * 100);
                    return returnValue;
                    acceptedInput = true;
                }
            }
        }

        //Now if there are more than 2 characters in the string, the first character will do the same as above, but the rest will be converted to an integer, and pretty much the same happens.
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

//This puts BLANK in every slot of the board that gets inputted.
void makeEmptyBoard(char board[M][N])
{
    for (int r = 0; r < M; r++) {
        for (int c = 0; c < N; c++) {
            board[r][c] = BLANK;
        }
    }
}

//This is the one that just puts ships wherever it feels like on the board.
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

//This prints the board it gets, with some spacers. It will print whatever is in the 2d array.
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

//This one will print whatever is in it, except for the location of the ships, they will appear as blanks.
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

//This just writes some letters.
void writeLetters()
{
    std::cout << "    ";
    for (int i = 0; i < N; i++) {
        char output = i + 65;
        std::cout << output << "   ";
    }
    std::cout << std::endl;
}

//these two just get random numbers and returns them.
//Because there are two different, i could in theory play with a board that isn't a perfect square. To do this, just change M and N in my header file.
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

//This is the one you provided.
int letterToNumber(char letter) // only big letters
{
    return static_cast<int>(letter) - 65;
}

//Pretty simple main menu.
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

//If the user wants to change passwords, they have to enter the one they have, and then enter the new one twice.
void changePassword()
{
    //The old (current) password first gets saved.
    std::vector <int> oldPassword{};
    oldPassword = readPassFromFile();
    std::vector <int> oldPasswordInput{};
    bool confirmedOldPass = true;

    //Make sure it at least happens once.
    do {
        //The password is confirmed until proven otherwise.
        confirmedOldPass = true;

        //Save the attempted password typed in from the user.
        oldPasswordInput = typingInPassword(loginScreen(), loggingIn);

        //Make sure i don't get the vector subscript out of range error, by checking that the passwords are the same size. 
        //If they aren't the same size, it's the wrong password anyways and we don't need to check it.
        if (oldPassword.size() == oldPasswordInput.size()) {
            //Then go through every character and check that it matches the current password.
            for (int i = 0; i < oldPassword.size(); i++) {
                //If one character doesn't match, the password is wrong and the loop will go again.
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

    //Then basically do the same again, except instead of comparing with the file, we compare the two inputted passwords.
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

        //If they match, overwrite the password.txt file and save the new password.
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

//This is what controls the characters on the login screen. They are stored as integers, but displayed as characters.
//The function distributes the array so whatever size the user chooses, the values increases one by one.
//For example:
/*
{
{1, 2, 3, 4 },
{5, 6, 7, 8 },
{9, 10, 11, 12},
{13, 14, 15, 16}
}
*/
std::vector <std::vector <int>> loginScreen() {
    std::vector <std::vector <int>> screen(loginGridSizeY, std::vector <int>(loginGridSizeX, 0));
    for (int r = 0; r < screen.size(); r++) {
        for (int c = 0; c < screen[r].size(); c++) {
            screen[r][c] = (r * screen[r].size()) + (c + 1);
        }
    }
    return screen;
}

//This is the main login function. it will return a value; either an error or a succesful log in.
int login() {

    //if the gridsize exceeds the amount of characters in the alphabet, it will not run.
    if (loginGridSizeX * loginGridSizeY > 26) {
        return 2;
    }

    //Store whatever is in the password file. If there is nothing, it will store nothing.
    std::vector <int> inputFromFile = {};
    inputFromFile = readPassFromFile();

    //If it is above the minimum password size, it means a password exists. if it's not, then this will run the makeFirstPassword function.
    if (inputFromFile.size() < minimumPasswordSize) {
        makeFirstPassword(loginScreen());
    }

    //Now even after making the password the user will have to log in.
    //First read the password from the file.
    inputFromFile = readPassFromFile();
    bool passwordsMatch = true;
    system("cls");

    //Then get the user's attempt.
    std::vector <int> passHolder = typingInPassword(loginScreen(), loggingIn);

    //and like above, compare them. if they don't match, the function will end. However, in the main() function, it loops until a succesful login.
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

//this function reads the file and returns a vector with integers that represent the characters you see on screen. A = 1, B = 2 etc.
//So if the password was ABC this will return {1, 2, 3}
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

//This will guide the user through making the first password.
//It's basically the same as above, just type it in twice, compare them, if they match, congrats that's the password.
void makeFirstPassword(std::vector <std::vector <int>> screen) {
    bool acceptedPassword = false;
    while (!acceptedPassword) {
        std::ofstream passwordFile;
        std::vector <int> passInput = {};
        std::vector <int> comparePassword = {};
        std::string passOutput = "";
        passInput = typingInPassword(screen, makeInitialPassword);

        //here though, i explored a different option. Rather than storing them in 2 vectors, i save one of them on the file, and then compare.
        //This is pretty stupid, but i wanted to see if it worked.
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

        //If the passwords don't match, the file gets overwritten, so the attempted password will not get saved.
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

//Just like the one that reads from the file, this also returns an integer vector. 
//This handles all the typing, and will display different information depending on which context it is used in.
std::vector <int> typingInPassword(std::vector <std::vector <int>> screen, int scenario) {

    //A cursor and a move direction is made
    std::vector <int> passInput;
    int moveDirection[2] = { 0, 0 };
    int cursor[2] = { 0, 0 };
    bool doneTyping = false;



    while (!doneTyping) {

        //Always save the start position.
        if (passInput.size() == 0) {
            passInput.push_back(screen[cursor[0]][cursor[0]]);
        }
        system("cls");

        //Display relevant information depending on what the user is doing.
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

        //Then print the board. Converts from numbers to characters.
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

        //Then store the attempted direction of movement in moveDirection, either in rows or columns.
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
                //if the user hits enter too early, they have to start over.
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

        //If the user is not done, and there exists a movement in some direction. (It will not if the user accidentally hits another key. So this is to stop it from saving the same character twice.)
        if (!doneTyping && (moveDirection[0] != 0 || moveDirection[1] != 0)) {
            //Then make sure the attempted move is actually on the screen so we don't go outside.
            if ((cursor[0] + moveDirection[0] < screen.size()) && (cursor[0] + moveDirection[0] >= 0) && (cursor[1] + moveDirection[1] < screen[0].size()) && (cursor[1] + moveDirection[1] >= 0)) {
                //Then update the cursors position.
                cursor[0] += moveDirection[0];
                cursor[1] += moveDirection[1];
                //and reset the movement direction.
                moveDirection[0] = 0;
                moveDirection[1] = 0;
                //then add the position from the array to the password vector.
                passInput.push_back(screen[cursor[0]][cursor[1]]);
            }
            //If the attempted position is outside the board, it will not move, and will reset movedirection.
            else {
                moveDirection[0] = 0;
                moveDirection[1] = 0;
            }
        }
    }

    return passInput;
}