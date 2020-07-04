// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header!

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;



void countNeighbours(int row, int col, Grid<bool> const & cellColony, Grid<int> &neighbourCount);

void printMatrix(Grid<bool>& cellColony, LifeGUI &gui) {

    gui.resize(cellColony.numRows(), cellColony.numCols());
    for(int i = 0; i < cellColony.numRows(); i++) {
        for(int j = 0; j < cellColony.numCols(); j++) {
           if(cellColony[i][j]) {
            cout << 'X';
            gui.drawCell(i, j, true);
           } else {
               cout << '-';
               gui.drawCell(i, j, false);
           }
        }
     cout<< endl;
    }
}


void mapping(Grid<bool>& cellColony, Grid<int>& neighbourCount) {
    for(int i = 0; i < cellColony.numRows(); i++) {
        for(int j = 0; j < cellColony.numCols(); j++) {
            if(neighbourCount[i][j] >= 0 && neighbourCount[i][j] <= 1) { // Cell Dies due to low popuation
                cellColony[i][j] = false;
            } else if(neighbourCount[i][j] == 3) { // Cell is born.
                cellColony[i][j] = true;
            } else if(neighbourCount[i][j] > 3) { // Cell dies due to over-crowding.
                cellColony[i][j] = false;
            }
        }
    }
}

void traversal(int num_gens, Grid<bool>& cellColony, Grid<int>& neighbourCount) {
    LifeGUI gui;
    while(num_gens--) {
        for(int i = 0; i < cellColony.numRows(); i++) {
            for(int j = 0; j < cellColony.numCols(); j++) {
                countNeighbours(i, j, cellColony, neighbourCount);
            }
         }
        mapping(cellColony, neighbourCount);
        printMatrix(cellColony, gui);
        neighbourCount.fill(0);
        if(num_gens != 0) {
           clearConsole();
        }
    }


}
void optionsForUser(Grid<bool>& cellColony,  Grid<int>& neighbourCount) {
    char ch;
    bool ans = true;
    while(ans) {
        cout << "Enter Choice: " << endl;
        cout << "Press 't' To Traverse 1 Generation : " << endl;
        cout << "Press 'a' To Animate Through A Series Of Generation: " << endl;
        cout << "Press 'q' To Quit The Program: " << endl;
        cin >> ch;
        switch(ch) {
            case 't' :
                traversal(1, cellColony, neighbourCount);
                break;
            case 'a' : {
                int num_gens = 0;
                cout << " Enter Number of Generations: ";
                cin >> num_gens;
                while(std::cin.fail()) {
                    std::cout << " Invalid: Enter Again:" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(256,'\n');
                    std::cin >> num_gens;
                }
                traversal(num_gens, cellColony, neighbourCount);
                break;
            }

            case 'q':
                return;

            default:
                char temp_ans;
                cout << " Invalid Input: " << endl;
                cout << " Want To Try Again ? " << endl;
                cout << " Press 'y' or 'n': ";
                cin >> temp_ans;
                if(temp_ans == 'n')
                    ans = false;
                else
                    continue;
            }
        break;
    }
}
void initializationFromFile(string file_name, Grid<bool>& cellColony,  Grid<int>& neighbourCount) {
    int num_rows = 0, num_cols = 0;
    ifstream fin;
    string readline;
    openFile(fin, file_name);
    getline(fin, readline);
    num_rows = stringToInteger(readline);
    getline(fin, readline);
    num_cols = stringToInteger(readline);
    cellColony.resize(num_rows, num_cols);
    neighbourCount.resize(num_rows, num_cols);
    for(int i = 0; i < num_rows; i++) {
        getline(fin, readline);
        for(int j = 0; j < num_cols; j++) {
            if(readline[j] == 'X') {// Checking For Live Cell
                cellColony[i][j] = true;
            }
            else {
                cellColony[i][j] = false;
            }
            neighbourCount[i][j] = 0;
        }
    }
    fin.close();

}
void countNeighbours(int row, int col, Grid<bool> const & cellColony, Grid<int> &neighbourCount) {
    for(int i = row - 1; i <= row + 1; ++i) {
        for(int j = col - 1; j <= col + 1; ++j) {
            if(cellColony.inBounds(i, j) && ((row != i || col != j) && cellColony[i][j])) {
                neighbourCount[row][col]++;
            }
        }
    }
}

int main() {
    // TODO: Finish the program!
    string file_name;
    file_name = getLine("Enter File Name");
    while (!fileExists(file_name)) {
        cout << "Please Enter Again: "<< endl;
        file_name = getLine("Enter File Name");
    }
    Grid<bool> cellColony(0 , 0);
    Grid<int> neighbourCount(0, 0);
    initializationFromFile(file_name, cellColony, neighbourCount);
    optionsForUser(cellColony, neighbourCount);
    cout << "Have a nice Life!" << endl;
    return 0;
}







