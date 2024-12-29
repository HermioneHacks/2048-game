#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

const int GRID_SIZE = 4;

// Function to display the grid
void displayGrid(const vector<vector<int> > &grid) {
    system("clear"); // Change to "cls" if on Windows
    for (vector<vector<int> >::const_iterator row = grid.begin(); row != grid.end(); row++) {
        for (vector<int>::const_iterator it = row->begin(); it != row->end(); it++) {
            cout << setw(5) << (*it ? to_string(*it) : ".");
        }
        cout << "\n";
    }
}

// Check if there are valid moves left
bool isMovePossible(const vector<vector<int> > &grid) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == 0) return true;
            if (i < GRID_SIZE - 1 && grid[i][j] == grid[i + 1][j]) return true;
            if (j < GRID_SIZE - 1 && grid[i][j] == grid[i][j + 1]) return true;
        }
    }
    return false;
}

// Add a new tile (2 or 4) to the grid
void addTile(vector<vector<int> > &grid) {
    vector<pair<int, int> > emptyPositions;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == 0) emptyPositions.push_back(make_pair(i, j));
        }
    }
    if (!emptyPositions.empty()) {
        pair<int, int> position = emptyPositions[rand() % emptyPositions.size()];
        int x = position.first;
        int y = position.second;
        grid[x][y] = (rand() % 2 + 1) * 2;
    }
}

// Slide and merge a single row
void processRow(vector<int> &row) {
    vector<int> result(GRID_SIZE, 0);
    int currentIndex = 0;
    for (int i = 0; i < GRID_SIZE; i++) {
        if (row[i] != 0) {
            if (currentIndex > 0 && result[currentIndex - 1] == row[i]) {
                result[currentIndex - 1] *= 2;
            } else {
                result[currentIndex++] = row[i];
            }
        }
    }
    row = result;
}

// Move grid left
void moveLeft(vector<vector<int> > &grid) {
    for (vector<vector<int> >::iterator row = grid.begin(); row != grid.end(); row++) {
        processRow(*row);
    }
}

// Move grid right
void moveRight(vector<vector<int> > &grid) {
    for (vector<vector<int> >::iterator row = grid.begin(); row != grid.end(); row++) {
        reverse(row->begin(), row->end());
        processRow(*row);
        reverse(row->begin(), row->end());
    }
}

// Move grid up
void moveUp(vector<vector<int> > &grid) {
    for (int col = 0; col < GRID_SIZE; col++) {
        vector<int> column(GRID_SIZE);
        for (int row = 0; row < GRID_SIZE; row++) {
            column[row] = grid[row][col];
        }
        processRow(column);
        for (int row = 0; row < GRID_SIZE; row++) {
            grid[row][col] = column[row];
        }
    }
}

// Move grid down
void moveDown(vector<vector<int> > &grid) {
    for (int col = 0; col < GRID_SIZE; col++) {
        vector<int> column(GRID_SIZE);
        for (int row = 0; row < GRID_SIZE; row++) {
            column[row] = grid[row][col];
        }
        reverse(column.begin(), column.end());
        processRow(column);
        reverse(column.begin(), column.end());
        for (int row = 0; row < GRID_SIZE; row++) {
            grid[row][col] = column[row];
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    vector<vector<int> > grid(GRID_SIZE, vector<int>(GRID_SIZE, 0));

    // Initialize the grid with two random tiles
    addTile(grid);
    addTile(grid);

    char move;
    while (true) {
        displayGrid(grid);

        if (!isMovePossible(grid)) {
            cout << "Game Over!\n";
            break;
        }

        cout << "Enter move (W/A/S/D): ";
        cin >> move;
        vector<vector<int> > oldGrid = grid;

        switch (toupper(move)) {
            case 'W': moveUp(grid); break;
            case 'A': moveLeft(grid); break;
            case 'S': moveDown(grid); break;
            case 'D': moveRight(grid); break;
            default: cout << "Invalid input! Try again.\n"; continue;
        }

        if (grid != oldGrid) {
            addTile(grid);
        }
    }

    return 0;
}
