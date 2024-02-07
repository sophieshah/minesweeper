//#include <SFML/Audio.hpp>
//#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
//#include <SFML/Network.hpp>
//#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include<fstream>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <math.h>
#include <chrono>
#include <sstream>
using namespace std;

int** makeMines(int numMines,int rows,int cols) {
    int** mineArray = new int* [rows];
    for (int i = 0; i < rows; i++) {
        mineArray[i] = new int[cols];
        for (int j = 0; j < cols; j++) {
            mineArray[i][j] = 0;
        }
    }
    int upperRow = rows-1;
    int upperCol = cols-1;
    int lower = 0;
    srand(time(0));

    for (int i = 0; i < numMines; i++) {
        int row = (rand() % (upperRow - lower + 1)) + lower;
        int col = (rand() % (upperCol - lower + 1)) + lower;
        mineArray[row][col] = 9;

    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << mineArray[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
    cout << endl;
    return mineArray;
}

int** checkNeighbor(int** mineArray,int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (mineArray[i][j] >= 9) {
                if (i == 0 && j == 0) {
                    mineArray[i][j + 1]++;
                    mineArray[i + 1][j]++;
                    mineArray[i + 1][j + 1]++;
                }
                else if (i == rows-1 && j == 0) {
                    mineArray[i - 1][j]++;
                    mineArray[i][j + 1]++;
                    mineArray[i - 1][j + 1]++;
                }
                else if (i == rows-1 && j == cols-1) {
                    mineArray[i][j - 1]++;
                    mineArray[i - 1][j]++;
                    mineArray[i - 1][j - 1]++;
                }
                else if (i == 0 && j == cols-1) {
                    mineArray[i][j - 1]++;
                    mineArray[i + 1][j]++;
                    mineArray[i + 1][j - 1]++;
                }
                else if (i == 0) {
                    mineArray[i][j - 1]++;
                    mineArray[i][j + 1]++;
                    mineArray[i + 1][j - 1]++;
                    mineArray[i + 1][j]++;
                    mineArray[i + 1][j + 1]++;
                }
                else if (i == rows-1) {
                    mineArray[i][j - 1]++;
                    mineArray[i][j + 1]++;
                    mineArray[i - 1][j]++;
                    mineArray[i - 1][j - 1]++;
                    mineArray[i - 1][j + 1]++;
                }
                else if (j == 0) {
                    mineArray[i - 1][j]++;
                    mineArray[i - 1][j + 1]++;
                    mineArray[i][j + 1]++;
                    mineArray[i + 1][j + 1]++;
                    mineArray[i + 1][j]++;
                }
                else if (j == cols-1) {
                    mineArray[i - 1][j]++;
                    mineArray[i - 1][j - 1]++;
                    mineArray[i][j - 1]++;
                    mineArray[i + 1][j - 1]++;
                    mineArray[i + 1][j]++;
                }
                else {
                    mineArray[i][j - 1]++;
                    mineArray[i - 1][j - 1]++;
                    mineArray[i - 1][j]++;
                    mineArray[i - 1][j + 1]++;
                    mineArray[i][j + 1]++;
                    mineArray[i + 1][j + 1]++;
                    mineArray[i + 1][j]++;
                    mineArray[i + 1][j - 1]++;
                }
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << mineArray[i][j] << " ";
        }
        cout << endl;
    }
    return mineArray;
}

void getRowCol(int& y, int& x,int rows, int cols) {
    for (int i = 1; i < cols+1; i++) {
        if (x <= i * 32) {
            x = i - 1;
            break;
        }
    }
    for (int j = 1; j < rows+1; j++) {
        if (y <= j * 32) {
            y = j - 1;
            break;
        }
    }
}

struct Tile {
    bool isMine = false;
    bool hasFlag = false;
    bool isClicked = false;
    int row;
    int col;
    int num;
    Tile(int row, int col, int num) {
        this->row = row;
        this->col = col;
        this->num = num;
        if (num >= 9) {
            isMine = true;
        }
        else {
            isMine = false;
        }
        hasFlag = false;
        isClicked = false;
    }
    Tile() {
        row = 0;
        col = 0;
        num = 0;
        if (num >= 9) {
            isMine = true;
        }
        else {
            isMine = false;
        }
        hasFlag = false;
        isClicked = false;
    }
};

class Board {
private:
    int rows;
    int cols;
    sf::RenderWindow window;
    int** boardArray;
    Tile** tileArray;
    int** mineArray;
    Tile** mineTileArray;
    Tile** pausedArray;
    Tile** winArray;
public:
    Board(int rows, int cols) {
        this->rows = rows;
        this->cols = cols;
        tileArray = new Tile * [rows];
        mineTileArray = new Tile * [rows];
        pausedArray = new Tile * [rows];
        winArray = new Tile * [rows];
    }
    void makeBoardArray(int numMines) {
        mineArray = makeMines(numMines,rows,cols);

        mineTileArray = createTilesMines();

        boardArray = checkNeighbor(mineArray,rows,cols);

    }

    void createTilesBoard() {
        for (int i = 0; i < rows; i++) {
            tileArray[i] = new Tile[cols];
            for (int j = 0; j < cols; j++) {
                Tile t1(i, j, boardArray[i][j]);
                tileArray[i][j] = t1;
            }
        }
    }

    Tile** createTilesMines() {
        for (int i = 0; i < rows; i++) {
            mineTileArray[i] = new Tile[cols];
            for (int j = 0; j < cols; j++) {
                Tile t1(i, j, mineArray[i][j]);
                if (mineArray[i][j] == 9) {
                    t1.isClicked = true;
                }
                mineTileArray[i][j] = t1;
            }
        }
        return mineTileArray;
    }

    Tile** createTilesWin() {
        for (int i = 0; i < rows; i++) {
            winArray[i] = new Tile[cols];
            for (int j = 0; j < cols; j++) {
                Tile t1(i, j, mineArray[i][j]);
                if (mineArray[i][j] == 9) {
                    t1.hasFlag = true;
                    t1.isClicked = true;
                }
                else {
                    t1.isClicked = true;
                }
                winArray[i][j] = t1;
            }
        }
        return winArray;
    }

    Tile** createPauseArray() {
        for (int i = 0; i < rows; i++) {
            pausedArray[i] = new Tile[cols];
            for (int j = 0; j < cols; j++) {
                Tile t1(i, j, 0);
                t1.isClicked = true;
                pausedArray[i][j] = t1;
            }
        }
        return pausedArray;
    }

    int** getBoardArray() {
        return boardArray;
    }

    Tile** getTileArray() {
        return tileArray;
    }

    int** getMineArray() {
        return mineArray;
    }

    Tile** getMineTileArray() {
        return mineTileArray;
    }


};

void drawSingleTile(sf::RenderWindow& window1, int row, int col, Tile** boardArr) {
    sf::Texture texture;
    if (boardArr[row][col].isClicked) {
        if (boardArr[row][col].hasFlag) {
            if (!texture.loadFromFile("C:/Prog2Project3/files/images/flag.png"))
            {
                cout << "image not opened correctly" << endl;
            }
        }
        else if (boardArr[row][col].num == 0) {
            if (!texture.loadFromFile("C:/Prog2Project3/files/images/tile_revealed.png"))
            {
                cout << "image not opened correctly" << endl;
            }
        }
        else if (boardArr[row][col].num == 1) {
            if (!texture.loadFromFile("C:/Prog2Project3/files/images/number_1.png"))
            {
                cout << "image not opened correctly" << endl;
            }
        }
        else if (boardArr[row][col].num == 2) {
            if (!texture.loadFromFile("C:/Prog2Project3/files/images/number_2.png"))
            {
                cout << "image not opened correctly" << endl;
            }
        }
        else if (boardArr[row][col].num == 3) {
            if (!texture.loadFromFile("C:/Prog2Project3/files/images/number_3.png"))
            {
                cout << "image not opened correctly" << endl;
            }
        }
        else if (boardArr[row][col].num == 4) {
            if (!texture.loadFromFile("C:/Prog2Project3/files/images/number_4.png"))
            {
                cout << "image not opened correctly" << endl;
            }
        }
        else if (boardArr[row][col].num == 5) {
            if (!texture.loadFromFile("C:/Prog2Project3/files/images/number_5.png"))
            {
                cout << "image not opened correctly" << endl;
            }
        }
        else if (boardArr[row][col].num == 6) {
            if (!texture.loadFromFile("C:/Prog2Project3/files/images/number_6.png"))
            {
                cout << "image not opened correctly" << endl;
            }
        }
        else if (boardArr[row][col].num == 7) {
            if (!texture.loadFromFile("C:/Prog2Project3/files/images/number_7.png"))
            {
                cout << "image not opened correctly" << endl;
            }
        }
        else if (boardArr[row][col].num == 8) {
            if (!texture.loadFromFile("C:/Prog2Project3/files/images/number_8.png"))
            {
                cout << "image not opened correctly" << endl;
            }
        }
        else if (boardArr[row][col].num >= 9) {
            if (!texture.loadFromFile("C:/Prog2Project3/files/images/mine.png"))
            {
                cout << "image not opened correctly" << endl;
            }
        }
    }
    else {
        if (boardArr[row][col].hasFlag) {
            if (!texture.loadFromFile("C:/Prog2Project3/files/images/flag.png"))
            {
                cout << "image not opened correctly" << endl;
            }
        }
        else {
            if (!texture.loadFromFile("C:/Prog2Project3/files/images/tile_hidden.png"))
            {
                cout << "image not opened correctly" << endl;
            }
        }

    }


    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(32 * col, 32 * row);
    window1.draw(sprite);
}

void createTile(sf::RenderWindow& window1, int rows, int cols, int currRow, int currCol, Tile** tileArr) {


    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            drawSingleTile(window1, i, j, tileArr);
        }
    }

    /*for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            drawSingleTile(window1, i,j,boardArr);
        }
    }*/
}

//void printTileArray(Tile** mineArray) {
//
//    for (int i = 0; i < 16; i++) {
//        for (int j = 0; j < 25; j++) {
//
//            cout << mineArray[i][j].num << " ";
//        }
//        cout << endl;
//    }
//}
//
//void printIntArray(int** mineArray) {
//
//    for (int i = 0; i < 16; i++) {
//        for (int j = 0; j < 25; j++) {
//
//            cout << mineArray[i][j] << " ";
//        }
//        cout << endl;
//    }
//}


void displaySurrounding(Tile& tile, Tile** mineArray,int &numCorrect,int rows, int cols) {
    int i = tile.row;
    int j = tile.col;
    if (i == 0 && j == 0) {
        if (!mineArray[i][j + 1].isMine && !mineArray[i + 1][j].isMine && !mineArray[i + 1][j + 1].isMine) {
             mineArray[i][j + 1].isClicked = true;
             mineArray[i + 1][j].isClicked = true;
             mineArray[i + 1][j + 1].isClicked = true;

             numCorrect += 3;
            
            
            /*displaySurrounding(mineArray[i][j + 1], mineArray);
            displaySurrounding(mineArray[i + 1][j], mineArray);
            displaySurrounding(mineArray[i + 1][j + 1], mineArray);*/
        }
    }
    else if (i == rows-1 && j == 0) {
        if (!mineArray[i - 1][j].isMine && !mineArray[i][j + 1].isMine && !mineArray[i - 1][j + 1].isMine) {
            mineArray[i][j + 1].isClicked = true;
            mineArray[i - 1][j].isClicked = true;
            mineArray[i - 1][j + 1].isClicked = true;

            numCorrect += 3;
            /*

            displaySurrounding(mineArray[i][j + 1], mineArray);
            displaySurrounding(mineArray[i - 1][j], mineArray);
            displaySurrounding(mineArray[i - 1][j + 1], mineArray);*/
        }
    }
    else if (i == rows-1 && j == cols-1) {
        if (!mineArray[i][j - 1].isMine && !mineArray[i - 1][j].isMine && !mineArray[i - 1][j - 1].isMine) {
            mineArray[i][j - 1].isClicked = true;
            mineArray[i - 1][j].isClicked = true;
            mineArray[i - 1][j - 1].isClicked = true;

            numCorrect += 3;

            //displaySurrounding(mineArray[i][j - 1], mineArray);
            //displaySurrounding(mineArray[i - 1][j], mineArray);
            //displaySurrounding(mineArray[i - 1][j - 1], mineArray);

        }
    }
    else if (i == 0 && j == cols-1) {
        if (!mineArray[i][j - 1].isMine && !mineArray[i + 1][j].isMine && !mineArray[i + 1][j - 1].isMine) {
            mineArray[i][j - 1].isClicked = true;
            mineArray[i + 1][j].isClicked = true;
            mineArray[i + 1][j - 1].isClicked = true;
            
            numCorrect += 3;
            
            /*

            displaySurrounding(mineArray[i][j - 1], mineArray);
            displaySurrounding(mineArray[i + 1][j], mineArray);
            displaySurrounding(mineArray[i + 1][j - 1], mineArray);*/

        }
    }
    else if (i == 0) {
        if (!mineArray[i][j - 1].isMine && !mineArray[i][j + 1].isMine && !mineArray[i + 1][j - 1].isMine
            && !mineArray[i + 1][j].isMine && !mineArray[i + 1][j + 1].isMine) {

            mineArray[i][j - 1].isClicked = true;
            mineArray[i][j + 1].isClicked = true;
            mineArray[i + 1][j - 1].isClicked = true;
            mineArray[i + 1][j].isClicked = true;
            mineArray[i + 1][j + 1].isClicked = true;

            numCorrect += 5;

            /*displaySurrounding(mineArray[i][j - 1], mineArray);
            displaySurrounding(mineArray[i][j+1], mineArray);
            displaySurrounding(mineArray[i + 1][j - 1], mineArray);
            displaySurrounding(mineArray[i + 1][j], mineArray);
            displaySurrounding(mineArray[i + 1][j + 1], mineArray);*/

        }
    }
    else if (i == rows-1) {
        if (!mineArray[i][j - 1].isMine && !mineArray[i][j + 1].isMine && !mineArray[i - 1][j].isMine
            && !mineArray[i - 1][j - 1].isMine && !mineArray[i - 1][j + 1].isMine) {

            mineArray[i][j - 1].isClicked = true;
            mineArray[i][j + 1].isClicked = true;
            mineArray[i - 1][j].isClicked = true;
            mineArray[i - 1][j - 1].isClicked = true;
            mineArray[i - 1][j + 1].isClicked = true;

            numCorrect += 5;

            /*displaySurrounding(mineArray[i][j - 1], mineArray);
            displaySurrounding(mineArray[i][j + 1], mineArray);
            displaySurrounding(mineArray[i - 1][j], mineArray);
            displaySurrounding(mineArray[i - 1][j-1], mineArray);
            displaySurrounding(mineArray[i - 1][j + 1], mineArray);*/

        }
    }
    else if (j == 0) {
        if (!mineArray[i - 1][j].isMine && !mineArray[i - 1][j + 1].isMine && !mineArray[i][j + 1].isMine
            && !mineArray[i + 1][j + 1].isMine && !mineArray[i + 1][j].isMine) {

            mineArray[i - 1][j].isClicked = true;
            mineArray[i - 1][j + 1].isClicked = true;
            mineArray[i][j + 1].isClicked = true;
            mineArray[i + 1][j + 1].isClicked = true;
            mineArray[i + 1][j].isClicked = true;

            numCorrect += 5;

            /*displaySurrounding(mineArray[i-1][j], mineArray);
            displaySurrounding(mineArray[i-1][j + 1], mineArray);
            displaySurrounding(mineArray[i][j + 1], mineArray);
            displaySurrounding(mineArray[i + 1][j+1], mineArray);
            displaySurrounding(mineArray[i + 1][j], mineArray);*/

        }
    }
    else if (j == cols-1) {
        if (!mineArray[i - 1][j].isMine && !mineArray[i - 1][j - 1].isMine && !mineArray[i][j - 1].isMine
            && !mineArray[i + 1][j - 1].isMine && !mineArray[i + 1][j].isMine) {

            mineArray[i - 1][j].isClicked = true;
            mineArray[i - 1][j - 1].isClicked = true;
            mineArray[i][j - 1].isClicked = true;
            mineArray[i + 1][j - 1].isClicked = true;
            mineArray[i + 1][j].isClicked = true;

            numCorrect += 5;

            /*displaySurrounding(mineArray[i-1][j], mineArray);
            displaySurrounding(mineArray[i-1][j - 1], mineArray);
            displaySurrounding(mineArray[i][j - 1], mineArray);
            displaySurrounding(mineArray[i + 1][j-1], mineArray);
            displaySurrounding(mineArray[i + 1][j], mineArray);*/
        }
    }
    else {
        if (!mineArray[i][j - 1].isMine && !mineArray[i - 1][j - 1].isMine && !mineArray[i - 1][j].isMine
            && !mineArray[i - 1][j + 1].isMine && !mineArray[i][j + 1].isMine && !mineArray[i + 1][j + 1].isMine
            && !mineArray[i + 1][j].isMine && !mineArray[i + 1][j - 1].isMine) {

            mineArray[i][j - 1].isClicked = true;
            mineArray[i - 1][j - 1].isClicked = true;
            mineArray[i - 1][j].isClicked = true;
            mineArray[i - 1][j + 1].isClicked = true;
            mineArray[i][j + 1].isClicked = true;
            mineArray[i + 1][j + 1].isClicked = true;
            mineArray[i + 1][j].isClicked = true;
            mineArray[i + 1][j - 1].isClicked = true;

            numCorrect += 8;

            /*displaySurrounding(mineArray[i][j - 1], mineArray);
            displaySurrounding(mineArray[i-1][j - 1], mineArray);
            displaySurrounding(mineArray[i - 1][j], mineArray);
            displaySurrounding(mineArray[i - 1][j+1], mineArray);
            displaySurrounding(mineArray[i][j + 1], mineArray);
            displaySurrounding(mineArray[i + 1][j + 1], mineArray);
            displaySurrounding(mineArray[i + 1][j], mineArray);
            displaySurrounding(mineArray[i + 1][j - 1], mineArray);*/

        }
    }

    /*

    else {
        mineArray[i][j - 1]++;
        mineArray[i - 1][j - 1]++;
        mineArray[i - 1][j]++;
        mineArray[i - 1][j + 1]++;
        mineArray[i][j + 1]++;
        mineArray[i + 1][j + 1]++;
        mineArray[i + 1][j]++;
        mineArray[i + 1][j - 1]++;
    }

    */
}

void drawButtons(sf::RenderWindow& window1, int col, int row, bool foundMine, int numCorrect, int numMine, bool pauseClicked, int numFlag, int mins, int secs, int numNeeded,bool gameWon) {
    //debug
    sf::Texture texture;
    if (!texture.loadFromFile("C:/Prog2Project3/files/images/debug.png"))
    {
        cout << "image not opened correctly" << endl;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(((col) * 32) - 304, 32 * ((row)+0.5f)
    );
    window1.draw(sprite);

    //happy face
   if (gameWon) {
        //cout << "correct" << endl;
        if (!texture.loadFromFile("C:/Prog2Project3/files/images/face_win.png"))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition((((col) / 2.0) * 32) - 32, 32 * ((row)+0.5f));
        window1.draw(sprite);
    }
   else if (!foundMine) {
       if (!texture.loadFromFile("C:/Prog2Project3/files/images/face_happy.png"))
       {
           cout << "image not opened correctly" << endl;
       }
       sprite.setTexture(texture);
       sprite.setPosition((((col) / 2.0) * 32) - 32, 32 * ((row)+0.5f));
       window1.draw(sprite);
    }
    else {
        if (!texture.loadFromFile("C:/Prog2Project3/files/images/face_lose.png"))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition((((col) / 2.0) * 32) - 32, 32 * ((row)+0.5f));
        window1.draw(sprite);
    }
    



    //pause/play
    if (!pauseClicked) {
        if (!texture.loadFromFile("C:/Prog2Project3/files/images/pause.png"))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(((col) * 32) - 240, 32 * ((row)+0.5f));
        window1.draw(sprite);
    }
    else {
        if (!texture.loadFromFile("C:/Prog2Project3/files/images/play.png"))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(((col) * 32) - 240, 32 * ((row)+0.5f));
        window1.draw(sprite);
    }


    //leaderboard
    if (!texture.loadFromFile("C:/Prog2Project3/files/images/leaderboard.png"))
    {
        cout << "image not opened correctly" << endl;
    }
    sprite.setTexture(texture);
    sprite.setPosition(((col) * 32) - 176, 32 * ((row)+0.5f));
    window1.draw(sprite);


    //counter
    if (numFlag < 0) {

        int first = abs(numFlag / 10);
        int second = abs(numFlag % 10);

        //cout << first << endl;
        if (!texture.loadFromFile("C:/Prog2Project3/files/images/digits.png", sf::IntRect(10 * 21, 0, 21, 32)))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(12, 32 * (row + 0.5f) + 16);
        window1.draw(sprite);


        if (!texture.loadFromFile("C:/Prog2Project3/files/images/digits.png", sf::IntRect(first * 21, 0, 21, 32)))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(33, 32 * (row + 0.5f) + 16);
        window1.draw(sprite);

        if (!texture.loadFromFile("C:/Prog2Project3/files/images/digits.png", sf::IntRect(second * 21, 0, 21, 32)))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(33 + 21, 32 * (row + 0.5f) + 16);
        window1.draw(sprite);
    }
    else {
        //cout << "positive flag count" << endl;
        int first = numFlag / 10;
        int second = numFlag % 10;

        //cout << first << endl;

        if (!texture.loadFromFile("C:/Prog2Project3/files/images/digits.png", sf::IntRect(first * 21, 0, 21, 32)))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(33, 32 * (row + 0.5f) + 16);
        window1.draw(sprite);

        if (!texture.loadFromFile("C:/Prog2Project3/files/images/digits.png", sf::IntRect(second * 21, 0, 21, 32)))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(33 + 21, 32 * (row + 0.5f) + 16);
        window1.draw(sprite);
    }

    //timer
    if (mins < 10) {
        if (!texture.loadFromFile("C:/Prog2Project3/files/images/digits.png", sf::IntRect(0 * 21, 0, 21, 32)))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition((col * 32) - 97, 32 * (row + 0.5f) + 16);
        window1.draw(sprite);

        if (!texture.loadFromFile("C:/Prog2Project3/files/images/digits.png", sf::IntRect(mins * 21, 0, 21, 32)))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition((col * 32) - 97 + 21, 32 * (row + 0.5f) + 16);
        window1.draw(sprite);
    }
    else {
        int first = mins / 10;
        int second = mins % 10;
        if (!texture.loadFromFile("C:/Prog2Project3/files/images/digits.png", sf::IntRect(first * 21, 0, 21, 32)))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition((col * 32) - 97, 32 * (row + 0.5f) + 16);
        window1.draw(sprite);

        if (!texture.loadFromFile("C:/Prog2Project3/files/images/digits.png", sf::IntRect(second * 21, 0, 21, 32)))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition((col * 32) - 97 + 21, 32 * (row + 0.5f) + 16);
        window1.draw(sprite);
    }


    if (secs < 10) {
        if (!texture.loadFromFile("C:/Prog2Project3/files/images/digits.png", sf::IntRect(0 * 21, 0, 21, 32)))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition((col * 32) - 54, 32 * (row + 0.5f) + 16);
        window1.draw(sprite);

        if (!texture.loadFromFile("C:/Prog2Project3/files/images/digits.png", sf::IntRect(secs * 21, 0, 21, 32)))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition((col * 32) - 54 + 21, 32 * (row + 0.5f) + 16);
        window1.draw(sprite);
    }
    else {
        int first = secs / 10;
        int second = secs % 10;
        if (!texture.loadFromFile("C:/Prog2Project3/files/images/digits.png", sf::IntRect(first * 21, 0, 21, 32)))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition((col * 32) - 54, 32 * (row + 0.5f) + 16);
        window1.draw(sprite);

        if (!texture.loadFromFile("C:/Prog2Project3/files/images/digits.png", sf::IntRect(second * 21, 0, 21, 32)))
        {
            cout << "image not opened correctly" << endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition((col * 32) - 54 + 21, 32 * (row + 0.5f) + 16);
        window1.draw(sprite);
    }


}


bool checkWin(int rows, int cols, Tile** tileArray) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!tileArray[i][j].isMine && !tileArray[i][j].isClicked) {
                return false;
            }
        }
    }
    return true;
}


void setText(sf::Text& text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));

}


int welcomeWindow(string& name) {
    float height = 600;
    float width = 800;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Welcome window");
    //sf::CircleShape shape(100.f);
    sf::RenderStates::Default;
    sf::Text welcomeText;
    sf::Font font;
    sf::Color color;

    font.loadFromFile("C:/Prog2Project3/files/font.ttf");
    welcomeText.setFont(font);
    welcomeText.setString("WELCOME TO MINESWEEPER");
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color(255, 255, 255));
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcomeText, width / 2.0f, height / 2.0f - 150);

    sf::Text inputPromptText;
    inputPromptText.setFont(font);
    inputPromptText.setString("Enter your name:");
    inputPromptText.setCharacterSize(20);
    inputPromptText.setFillColor(sf::Color(255, 255, 255));
    inputPromptText.setStyle(sf::Text::Bold);
    setText(inputPromptText, width / 2.0f, height / 2.0f - 75);

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setString("Enter your name:");
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Yellow);
    inputText.setStyle(sf::Text::Bold);
    setText(inputText, width / 2.0f, height / 2.0f - 45);


    string str;

    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {

                window.close();
                return -1;
            }
            if (event.key.code == sf::Keyboard::Enter) {
                cout << "enter" << endl;
                if (str.length() != 0) {
                    window.close();
                    return 1;
                }
                else {
                    cout << "nothing entered" << endl;
                }
            }
            else if (event.key.code == sf::Keyboard::Backspace) {
                cout << "backspace" << endl;
                if (str.length() != 0) {
                    str.erase(str.length() - 1);
                }
            }
            else if (event.type == sf::Event::TextEntered) {
                if (isalpha(event.text.unicode)) {
                    if (str.length() == 0) {
                        str += toupper(event.text.unicode);
                    }
                    else if (str.length() <= 9) {
                        str += tolower(event.text.unicode);
                    }
                }

               
            }
        }

        inputText.setString(str + "|");
        name = str;
        setText(inputText, width / 2.0f, height / 2.0f - 45);
        window.clear(sf::Color(0, 0, 255, 0));
        window.draw(welcomeText);
        window.draw(inputPromptText);
        window.draw(inputText);

        window.display();

    }
}

int openLeaderboard(string name, int mins1, int secs1, float height, float width) {

    sf::Font font;
    sf::Text leaderboardText;
    font.loadFromFile("C:/Prog2Project3/files/font.ttf");
    leaderboardText.setFont(font);
    leaderboardText.setString("LEADERBOARD");
    leaderboardText.setCharacterSize(20);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(leaderboardText, width / 4.0f, height / 4.0f - 120);


    sf::Text leaderboardNamesText;
    font.loadFromFile("C:/Prog2Project3/files/font.ttf");
    leaderboardNamesText.setFont(font);
    //leaderboardNamesText.setString("LEADERBOARD");
    leaderboardNamesText.setCharacterSize(18);
    leaderboardNamesText.setFillColor(sf::Color::White);
    leaderboardNamesText.setStyle(sf::Text::Bold);
    setText(leaderboardNamesText, width / 8.0f, height / 8.0f + 20);



    ifstream infile("C:/Prog2Project3/files/leaderboard.txt");

    string singleLine;
    string minTime;
    string secTime;
    int mins;
    int secs;
    string newName;


    string leaderboardNames;
    int numNames = 1;

    int time = mins1 * 100 + secs1;

    vector<int> leaderTimes;
    vector<string>leaderNames;

    map<int, string>leaderboardMap;
    map<int, string>::iterator iter;

    if (infile.is_open()) {
        cout << "opened correctly" << endl;
    }
    while (getline(infile, singleLine)) {
        istringstream newStream(singleLine);
        getline(newStream, minTime, ':');
        mins = stoi(minTime);
        getline(newStream, secTime, ',');
        secs = stoi(secTime);
        string newName;
        getline(newStream, newName);

        int totalTime = mins * 100 + secs;


        leaderTimes.push_back(totalTime);

        leaderboardMap[totalTime] = newName;

        numNames++;
    }

    leaderTimes.push_back(time);
    leaderboardMap[time] = name;

    sort(leaderTimes.begin(), leaderTimes.end());

    //cout << "sorted" << endl;

    //cout << "size of leadertimes" << leaderTimes.size() << endl;

    bool userTop5 = false;

    for (iter = leaderboardMap.begin(); iter != leaderboardMap.end(); iter++) {
        if (iter->first == leaderTimes[0]) {
            if (iter->second == name) {
                userTop5 = true;
                leaderNames.push_back(iter->second + "*");
            }
            else {
                leaderNames.push_back(iter->second);
            }

        }
        if (iter->first == leaderTimes[1]) {
            if (iter->second == name) {
                userTop5 = true;
                leaderNames.push_back(iter->second + "*");
            }
            else {
                leaderNames.push_back(iter->second);
            }
        }
        if (iter->first == leaderTimes[2]) {
            if (iter->second == name) {
                userTop5 = true;
                leaderNames.push_back(iter->second + "*");
            }
            else {
                leaderNames.push_back(iter->second);
            }
        }
        if (iter->first == leaderTimes[3]) {
            if (iter->second == name) {
                userTop5 = true;
                leaderNames.push_back(iter->second + "*");
            }
            else {
                leaderNames.push_back(iter->second);
            }
        }
        if (iter->first == leaderTimes[4]) {
            if (iter->second == name) {
                userTop5 = true;
                leaderNames.push_back(iter->second + "*");
            }
            else {
                leaderNames.push_back(iter->second);
            }
        }
    }

    //cout << "pushed names" << endl;

    for (int i = 0; i < 5; i++) {
        int topMins = leaderTimes[i] / 100;
        int topSecs = leaderTimes[i] % 100;
        if (topMins < 10) {
            if (topSecs < 10) {
                leaderboardNames += to_string(i + 1) + ".\t" + "0" + to_string(topMins) + ":0" + to_string(topSecs)
                    + "\t" + leaderNames[i] + "\n\n";
            }
            else {
                leaderboardNames += to_string(i + 1) + ".\t0" + to_string(topMins) + ":" + to_string(topSecs)
                    + "\t" + leaderNames[i] + "\n\n";
            }
        }
        else {
            if (topSecs < 10) {
                leaderboardNames += to_string(i + 1) + ".\t" + to_string(topMins) + ":0" + to_string(topSecs)
                    + "\t" + leaderNames[i] + "\n\n";
            }
            else {
                leaderboardNames += to_string(i + 1) + ".\t" + to_string(topMins) + ":" + to_string(topSecs)
                    + "\t" + leaderNames[i] + "\n\n";
            }
        }
        //cout << "adding to leaderboard" << endl;
    }


    cout << leaderboardNames << endl;


    infile.close();

    ofstream outfile("C:/Prog2Project3/files/leaderboard.txt", ios::app);

    if (userTop5) {
        outfile << mins1 << ":" << secs1 << "," << name << endl;
    }



    leaderboardNamesText.setString(leaderboardNames);

    sf::RenderWindow window(sf::VideoMode(width / 2.0f, height / 2.0f), "Leaderboard");

    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {

                window.close();
                return -1;
            }

        }

        window.clear(sf::Color::Blue);
        window.draw(leaderboardText);
        window.draw(leaderboardNamesText);
        window.display();

    }
    return 0;
}

void openGameWindow(float gameWidth, float gameHeight, int cols1, int rows1, int numMine, string name) {
    float height = gameHeight;
    float width = gameWidth;
    int rows = rows1;
    int cols = cols1;
    sf::RenderWindow window(sf::VideoMode(width, height), "Game window");

    //sf::Texture texture;
    Board b1(rows,cols);
    b1.makeBoardArray(numMine);
    b1.createTilesBoard();
    int** boardArray = b1.getBoardArray();
    Tile** tileArray = b1.getTileArray();
    Tile** mineArray = b1.getMineTileArray();
    Tile** pauseArray = b1.createPauseArray();
    Tile** winArray = b1.createTilesWin();

    int currRow = 0;
    int currCol = 0;

    //b1.createTilesBoard();

    bool debugIsClicked = false;
    int numFlag = numMine;
    bool foundMine = false;
    int numCorrect = 0;
    bool pauseClicked = false;
    bool leaderboardClicked = false;
    int correctNeeded = (cols1 * rows1) - numMine;
    //cout << "correct needed " << correctNeeded << endl;
    bool gameWon = false;


    auto start = chrono::high_resolution_clock::now();


    while (window.isOpen()) {

        sf::Event event;


        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {

                window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                cout << mousePos.x << " " << mousePos.y << endl;
                if (mousePos.x >= ((cols) * 32) - 304 && mousePos.x <= ((cols) * 32) - 304+60 && mousePos.y >= (rows * 32)+19 && mousePos.y <= (rows * 32) + 19+60) {
                    if (!gameWon) {
                        debugIsClicked = !debugIsClicked;
                        cout << "debug clicked " << debugIsClicked << endl;
                        /*if (debugIsClicked) {
                            printTileArray(mineArray);
                        }
                        */
                    }
                    
                }
                else if (mousePos.x >= ((cols) * 32) - 240 && mousePos.x <= ((cols) * 32) - 240+60 && mousePos.y >= (rows * 32) + 19 && mousePos.y <= (rows * 32) + 19 + 60) {
                    if (foundMine == false&&!gameWon && !foundMine) {
                        cout << "pause clicked" << endl;
                        pauseClicked = !pauseClicked;
                    }
                    
                    
                }
                else if (mousePos.x >= (((cols) / 2.0) * 32) - 32 && mousePos.x <= (((cols) / 2.0) * 32) - 32+60 && mousePos.y >= (rows * 32) + 19 && mousePos.y <= (rows * 32) + 19 + 60) {
                    cout << "happy face clicked" << endl;
                    start = chrono::high_resolution_clock::now();
                    b1.makeBoardArray(numMine);
                    b1.createTilesBoard();
                    boardArray = b1.getBoardArray();
                    tileArray = b1.getTileArray();
                    mineArray = b1.getMineTileArray();
                    winArray = b1.createTilesWin();
                    debugIsClicked = false;
                    numFlag = numMine;
                    foundMine = false;
                    numCorrect = 0;
                    pauseClicked = false;
                    gameWon = false;

                }
                else if (mousePos.x >= ((cols) * 32) - 176 && mousePos.x <= ((cols) * 32) - 176+60 && mousePos.y >= (rows * 32) + 19 && mousePos.y <= (rows * 32) + 19 + 60) {
                    cout << "leaderboard clicked"<<!leaderboardClicked << endl;
                    leaderboardClicked = !leaderboardClicked;


                }
                else if (mousePos.y <= (rows * 32) + 19) {
                    if (!pauseClicked && !gameWon) {
                        currRow = mousePos.y;
                        currCol = mousePos.x;
                        getRowCol(currRow, currCol,rows,cols);
                        if (!tileArray[currRow][currCol].hasFlag) {
                            tileArray[currRow][currCol].isClicked = true;
                            cout << currRow << " " << currCol << endl;
                            if (tileArray[currRow][currCol].num >= 9) {
                                foundMine = true;
                                cout << "found mine" << endl;
                            }
                            else {
                                cout << "drawn at " << currRow << currCol << endl;
                                displaySurrounding(tileArray[currRow][currCol], tileArray, numCorrect, rows, cols);
                                //cout << "num correct " << numCorrect;
                                tileArray[currRow][currCol].isClicked = true;
                            }
                        }
                        cout << "game won " << gameWon << endl;
                    }
                    
                }


            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (!pauseClicked && !gameWon) {
                    //cout << mousePos.x <<" "<<mousePos.y << endl;
                    currRow = mousePos.y;
                    currCol = mousePos.x;
                    getRowCol(currRow, currCol,rows,cols);
                    cout << currRow << " " << currCol << endl;
                    tileArray[currRow][currCol].hasFlag = !tileArray[currRow][currCol].hasFlag;
                    if (tileArray[currRow][currCol].hasFlag) {
                        tileArray[currRow][currCol].isClicked = false;
                        numFlag--;
                        cout << "flag drawn at " << currRow << currCol << endl;
                        cout << "num flag: " << numFlag << endl;
                    }
                    else {
                        numFlag++;
                        cout << "flag removed at " << currRow << currCol << endl;
                        cout << "num flag: " << numFlag << endl;
                    }
                }
                
            }
        }


        gameWon = checkWin(rows, cols, tileArray);

        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> duration = end - start;

        //cout << duration.count() << endl;

        auto min = chrono::duration_cast<chrono::minutes>(end - start);
        int mins = min.count();
        //cout << mins.count() << endl;

        auto sec = chrono::duration_cast<chrono::seconds>(end - start);
        int secs = sec.count();
        /*if (secs == 3) {
            gameWon = true;
        }*/
        //cout << secs.count() << endl;


        window.clear(sf::Color(255, 255, 255, 255));
        if (debugIsClicked) {
            if (!pauseClicked) {
                createTile(window, rows, cols, currRow, currCol, mineArray);
            }
            else {
                start = chrono::high_resolution_clock::now();
                end = chrono::high_resolution_clock::now();

                createTile(window, rows, cols, currRow, currCol, pauseArray);
            }
            //cout << "creating mines" << endl;
            
        }
        
        else if (leaderboardClicked) {
            createTile(window, rows, cols, currRow, currCol, pauseArray);
            if (openLeaderboard(name, 120, 560, height, width) == -1) {
                leaderboardClicked = false;
            }
            
            

        }
        else if (pauseClicked) {
            start = chrono::high_resolution_clock::now();
            end = chrono::high_resolution_clock::now();

            createTile(window, rows, cols, currRow, currCol, pauseArray);
        }
        else if (gameWon) {
            //cout << "game won" << endl;
            createTile(window, rows, cols, currRow, currCol, winArray);
            numFlag = 0;
        }
        else if (foundMine) {
            createTile(window, rows, cols, currRow, currCol, mineArray);
        }
        else {
            createTile(window, rows, cols, currRow, currCol, tileArray);
        }


        drawButtons(window, cols, rows, foundMine, numCorrect, numMine, pauseClicked, numFlag, mins, secs,correctNeeded,gameWon);
        
        //drawSingleTile(window, currRow,currCol);

        window.display();
    }
}

int main()
{
    string name;
    if (welcomeWindow(name) == -1) {
        return 0;
    }
    cout << name << endl;

    int numCol;
    int numRow;
    int numMine;
    fstream infile("C:/Prog2Project3/files/board_config.cfg");
    infile >> numCol;
    infile >> numRow;
    infile >> numMine;

    //CHANGE LATER
    //numCol = 30; 
    //numRow = 30;
    //numMine = 5;

    float width = numCol * 32;
    float height = (numRow * 32) + 100;

    

    cout << numCol << endl;
    cout << numRow << endl;
    cout << numMine << endl;



    

    openGameWindow(width, height, numCol, numRow, numMine, name);



    //openLeaderboard(name, 8,20, height, width);
    cout << "opening game window" << endl;

    return 0;
}