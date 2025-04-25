#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

const char EMPTY = '.';
const char SILVER_COIN = 'S';
const char GOLD_COIN = 'G';
const char HELPER = 'H';
const char CENTER = 'X';

class Board {
public:
    int size;
    vector<vector<char>> grid;

    Board(int s) : size(s) {
        grid.resize(size, vector<char>(size, EMPTY));
        placeItems(SILVER_COIN, size);  // Silver coins
        placeItems(GOLD_COIN, size / 2); // Gold coins
        placeItems(HELPER, size / 4);   // Helper objects
        grid[size / 2][size / 2] = CENTER; // Center mark
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
    int silver = 5, gold = 2, helpers = 1;
    int blockTurns = 0;
    char hurdleToPlace = ' ';
    int lastDir = 1;

    Player(string n, int r, int c) : name(n), row(r), col(c) {}

    pair<int, int> getPosition() {
        return { row, col };
    }

    void move(Board &b, bool isP1) {
        if (blockTurns > 0) {
            cout << name << " is blocked for " << blockTurns << " more turns!\n";
            blockTurns--;
            return;
        }

        if (b.grid[row][col] == SILVER_COIN) {
            silver++;
            b.grid[row][col] = EMPTY;
            cout << name << " collected a silver coin!\n";
        } else if (b.grid[row][col] == GOLD_COIN) {
            gold++;
            b.grid[row][col] = EMPTY;
            cout << name << " collected a gold coin!\n";
        } else if (b.grid[row][col] == HELPER) {
            helpers++;
            b.grid[row][col] = EMPTY;
            cout << name << " collected a helper object!\n";
        }

        char cell = b.grid[row][col];
        if (cell == 'F' || cell == 'S' || cell == 'G' || cell == 'L') {
            if (helpers > 0) {
                cout << "You encountered a hurdle (" << cell << "). Use helper object to skip it? (y/n): ";
                char ch;
                cin >> ch;
                if (ch == 'y' || ch == 'Y') {
                    helpers--;
                    b.grid[row][col] = EMPTY;
                    cout << "Helper used to skip the hurdle!\n";
                } else {
                    applyHurdleEffect(cell, b.size, isP1);
                }
            } else {
                applyHurdleEffect(cell, b.size, isP1);
            }
        }

        if (isP1) {
            if ((row % 2 == 0 && col + 1 < b.size) || (row % 2 != 0 && col - 1 >= 0)) {
                col += (row % 2 == 0) ? 1 : -1;
            } else {
                row--;
            }
        } else {
            if ((row % 2 == 0 && col - 1 >= 0) || (row % 2 != 0 && col + 1 < b.size)) {
                col += (row % 2 == 0) ? -1 : 1;
            } else {
                row++;
            }
        }
    }

    void applyHurdleEffect(char cell, int size, bool isP1) {
        if (cell == 'F') {
            blockTurns = 2;
            cout << name << " encountered Fire! Blocked for 2 turns.\n";
        } else if (cell == 'S') {
            blockTurns = 3;
            cout << name << " encountered Snake! Blocked for 3 turns and pushed back.\n";
            for (int i = 0; i < 3; ++i) moveBack(size, isP1);
        } else if (cell == 'G') {
            blockTurns = 1;
            cout << name << " encountered Ghost! Blocked for 1 turn.\n";
        } else if (cell == 'L') {
            blockTurns = 4;
            cout << name << " encountered Lion! Blocked for 4 turns.\n";
        }
    }

    void moveBack(int size, bool isP1) {
        if (isP1) {
            if ((row % 2 == 0 && col - 1 >= 0) || (row % 2 != 0 && col + 1 < size)) {
                col += (row % 2 == 0) ? -1 : 1;
            } else {
                row++;
            }
        } else {
            if ((row % 2 == 0 && col + 1 < size) || (row % 2 != 0 && col - 1 >= 0)) {
                col += (row % 2 == 0) ? 1 : -1;
            } else {
                row--;
            }
        }
    }

    void buyAndPlaceHurdle(Board &b) {
        cout << "Available options: F(5 silver), S(3 silver), G(2 silver), L(5 gold), H(2 silver - Helper object)\n";
        cout << "You have " << silver << " silver coins, " << gold << " gold coins, and " << helpers << " helper objects.\n";
        cout << "Enter item to buy: ";
        char h;
        cin >> h;

        if ((h == 'F' && silver >= 5) || (h == 'S' && silver >= 3) || (h == 'G' && silver >= 2) || (h == 'L' && gold >= 5) || (h == 'H' && silver >= 2)) {
            if (h == 'H') {
                helpers++;
                silver -= 2;
                cout << "Bought a helper object.\n";
                return;
            }

            int r, c;
            cout << "Enter coordinates to place hurdle: ";
            cin >> r >> c;
            b.placeHurdle(r, c, h);
            if (h == 'L') gold -= 5;
            else if (h == 'F') silver -= 5;
            else if (h == 'S') silver -= 3;
            else if (h == 'G') silver -= 2;
        } else {
            cout << "Insufficient coins or invalid item!\n";
        }
    }

    bool atCenter(int center) {
        return row == center && col == center;
    }
};

void playGame(int boardSize) {
    Board board(boardSize);
    Player p1("Player 1", boardSize - 1, 0);
    Player p2("Player 2", 0, boardSize - 1);
    bool p1Turn = true;

    while (true) {
        system("cls");
        board.displayBoard(p1.getPosition(), p2.getPosition());
        Player &current = p1Turn ? p1 : p2;
        bool isP1 = p1Turn;

        cout << current.name << "'s turn:\n1. Move\n2. Buy & Place Item\nChoose: ";
        int choice;
        cin >> choice;

        if (choice == 1) current.move(board, isP1);
        else if (choice == 2) current.buyAndPlaceHurdle(board);
        else cout << "Invalid choice.\n";

        if (current.atCenter(boardSize / 2)) {
            cout << current.name << " reached the center and wins!\n";
            break;
        }

        p1Turn = !p1Turn;
        system("pause");
    }
}

int main() {
    srand(time(0));
    int size;
    cout << "Enter board size (odd number): ";
    cin >> size;
    if (size % 2 == 0) size++;
    playGame(size);
    return 0;
}