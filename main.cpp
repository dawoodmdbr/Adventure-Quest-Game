#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

const char EMPTY = '.';
const char GOLD = 'G';
const char SILVER = 'S';
const char HELPER = 'H';
const char CENTER = 'X';

class Board {
public:
    int size;
    vector<vector<char>> grid;

    Board(int s) : size(s) {
        grid.resize(size, vector<char>(size, EMPTY));
        placeItems(GOLD, size / 3);     // Gold coins
        placeItems(SILVER, size / 2);   // Silver coins
        placeItems(HELPER, size / 4);   // Helpers
        grid[size / 2][size / 2] = CENTER; // Center cell
    }

    void placeItems(char item, int count) {
        while (count--) {
            int r = rand() % size;
            int c = rand() % size;
            if (grid[r][c] == EMPTY)
                grid[r][c] = item;
            else
                count++;
        }
    }

    void placeHurdle(int row, int col, char symbol) {
        if (isValidCell(row, col) && grid[row][col] == EMPTY)
            grid[row][col] = symbol;
        else
            cout << "Invalid cell for hurdle.\n";
    }

    bool isValidCell(int r, int c) {
        return r >= 0 && r < size && c >= 0 && c < size;
    }

    void displayBoard(pair<int, int> p1, pair<int, int> p2) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (p1.first == i && p1.second == j)
                    cout << "1 ";
                else if (p2.first == i && p2.second == j)
                    cout << "2 ";
                else
                    cout << grid[i][j] << " ";
            }
            cout << endl;
        }
    }
};

class Player {
public:
    string name;
    int row, col;
    int points = 10;
    int helpers = 0;
    char hurdleToPlace = ' ';
    bool isPlayer1;

    Player(string n, int r, int c, bool p1) : name(n), row(r), col(c), isPlayer1(p1) {}

    pair<int, int> getPosition() {
        return { row, col };
    }

    void move(Board &b) {
        char cell = b.grid[row][col];
        if (cell == GOLD) {
            points += 10;
            b.grid[row][col] = EMPTY;
            cout << name << " collected a Gold Coin (+10 points)!\n";
        } else if (cell == SILVER) {
            points += 5;
            b.grid[row][col] = EMPTY;
            cout << name << " collected a Silver Coin (+5 point)!\n";
        } else if (cell == HELPER) {
            helpers++;
            b.grid[row][col] = EMPTY;
            cout << name << " collected a helper object!\n";
        } else if (cell == 'F' || cell == 'S' || cell == 'G' || cell == 'L' || cell == 'K') {
            cout << name << " encountered a hurdle (" << b.grid[row][col] << ")! Lost a turn.\n";
            return; // lose turn
        }

        // Zigzag movement
        if (isPlayer1) {
            if ((b.size - 1 - row) % 2 == 0) { // even row from bottom
                if (col + 1 < b.size)
                    col++;
                else
                    row--;
            } else {
                if (col - 1 >= 0)
                    col--;
                else
                    row--;
            }
        } else {
            if (row % 2 == 0) { // even row from top
                if (col - 1 >= 0)
                    col--;
                else
                    row++;
            } else {
                if (col + 1 < b.size)
                    col++;
                else
                    row++;
            }
        }
    }

    void placeHurdle(Board &b) {
        int r, c;
        cout << name << ", enter coordinates to place hurdle (" << hurdleToPlace << "): ";
        cin >> r >> c;
        b.placeHurdle(r, c, hurdleToPlace);
        hurdleToPlace = ' ';
    }

    void buyAndPlaceHurdle(Board &b) {
        cout << "Available hurdles: F (Fire), S (Snake), G (Ghost), L (Lion), K (Lock)\n";
        cout << "Each costs 2 points. You have " << points << " points.\n";
        cout << "Enter symbol of hurdle to buy: ";
        char h;
        cin >> h;
        if (points >= 2 && (h == 'F' || h == 'S' || h == 'G' || h == 'L' || h == 'K')) {
            hurdleToPlace = h;
            points -= 2;
            cout << "Bought hurdle: " << h << ". Now place it on the board.\n";
            placeHurdle(b);
        } else {
            cout << "Not enough points or invalid symbol.\n";
        }
    }

    bool atCenter(int center) {
        return row == center && col == center;
    }
};

void playGame(int boardSize) {
    Board board(boardSize);
    Player p1("Player 1", boardSize - 1, 0, true);
    Player p2("Player 2", 0, boardSize - 1, false);
    bool p1Turn = true;

    while (true) {
        board.displayBoard(p1.getPosition(), p2.getPosition());
        Player &current = p1Turn ? p1 : p2;
        cout << current.name << "'s turn (Points: " << current.points << ")\n";
        cout << "1. Move\n2. Buy and Place Hurdle\nChoose action: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: current.move(board); break;
            case 2: current.buyAndPlaceHurdle(board); break;
            default: cout << "Invalid choice.\n"; continue;
        }

        if (current.atCenter(board.size / 2)) {
            cout << current.name << " reached the center and won the game!\n";
            break;
        }

        p1Turn = !p1Turn;
    }
}

int main() {
    srand(time(0));
    int size;
    cout << "Enter board size (odd number): ";
    cin >> size;
    if (size % 2 == 0) size += 1;
    playGame(size);
    return 0;
}
