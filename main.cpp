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

class Board
{
public:
    int size;
    vector<vector<char>> grid;

    Board(int s) : size(s)
    {
        grid.resize(size, vector<char>(size, EMPTY));
        placeItems(SILVER_COIN, size);
        placeItems(GOLD_COIN, size / 2);
        placeItems(HELPER, size / 4);
        grid[size / 2][size / 2] = CENTER;
    }

    void placeItems(char item, int count)
    {
        while (count--)
        {
            int r = rand() % size;
            int c = rand() % size;

            if ((r == size - 1 && c == 0) || (r == 0 && c == size - 1) || grid[r][c] != EMPTY)
            {
                count++;
                continue;
            }

            grid[r][c] = item;
        }
    }

    void placeHurdle(int row, int col, char symbol)
    {
        if (!isValidCell(row, col))
        {
            cout << "Invalid cell coordinates.\n";
            return;
        }

        if ((row == size - 1 && col == 0) || (row == 0 && col == size - 1))
        {
            cout << "Cannot place hurdle on a player's starting position.\n";
            return;
        }

        if (grid[row][col] == CENTER)
        {
            cout << "Cannot place hurdle on the center tile.\n";
            return;
        }

        if (grid[row][col] == SILVER_COIN || grid[row][col] == GOLD_COIN || grid[row][col] == HELPER)
        {
            cout << "Note: Existing item at (" << row << "," << col << ") was removed to place hurdle.\n";
        }

        grid[row][col] = symbol;
    }

    bool isValidCell(int r, int c)
    {
        return r >= 0 && r < size && c >= 0 && c < size;
    }

    void displayBoard(pair<int, int> p1, pair<int, int> p2)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
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

class Player
{
public:
    string name;
    int row, col;
    int silver = 5, gold = 2, helpers = 1;
    int blockTurns = 0;
    char hurdleToPlace = ' ';
    int lastDir = 1;

    Player(string n, int r, int c) : name(n), row(r), col(c) {}

    pair<int, int> getPosition()
    {
        return {row, col};
    }
    string getHurdleName(char symbol)
    {
        switch (symbol)
        {
        case 'F':
            return "Fire";
        case 'S':
            return "Snake";
        case 'G':
            return "Ghost";
        case 'L':
            return "Lion";
        default:
            return "Unknown";
        }
    }

    void move(Board &b, bool isP1)
    {
        if (blockTurns > 0)
        {
            cout << name << " is blocked for " << blockTurns << " more turns!" << endl;
            blockTurns--;
            return;
        }

        if (isP1)
        {
            if ((row % 2 == 0 && col + 1 < b.size) || (row % 2 != 0 && col - 1 >= 0))
            {
                col += (row % 2 == 0) ? 1 : -1;
            }
            else
            {
                row--;
            }
        }
        else
        {
            if ((row % 2 == 0 && col - 1 >= 0) || (row % 2 != 0 && col + 1 < b.size))
            {
                col += (row % 2 == 0) ? -1 : 1;
            }
            else
            {
                row++;
            }
        }

        if (b.grid[row][col] == SILVER_COIN)
        {
            silver++;
            b.grid[row][col] = EMPTY;
            cout << name << " collected a silver coin!" << endl;
        }
        else if (b.grid[row][col] == GOLD_COIN)
        {
            gold++;
            b.grid[row][col] = EMPTY;
            cout << name << " collected a gold coin!" << endl;
        }
        else if (b.grid[row][col] == HELPER)
        {
            helpers++;
            b.grid[row][col] = EMPTY;
            cout << name << " collected a helper object!" << endl;
        }

        char cell = b.grid[row][col];
        if (cell == 'F' || cell == 'S' || cell == 'G' || cell == 'L')
        {
            if (helpers > 0)
            {
                cout << "You encountered a hurdle (" << getHurdleName(cell) << "). Use helper object to skip it? (y/n): ";
                char ch;
                cin >> ch;
                if (ch == 'y' || ch == 'Y')
                {
                    helpers--;
                    b.grid[row][col] = EMPTY;
                    cout << "Helper used to skip the hurdle!" << endl;
                }
                else
                {
                    applyHurdleEffect(cell, b.size, isP1);
                }
            }
            else
            {
                applyHurdleEffect(cell, b.size, isP1);
            }
        }
    }

    void applyHurdleEffect(char cell, int size, bool isP1)
    {
        if (cell == 'F')
        {
            blockTurns = 2;
            cout << name << " encountered Fire! Blocked for 2 turns." << endl;
        }
        else if (cell == 'S')
        {
            blockTurns = 3;
            cout << name << " encountered Snake! Blocked for 3 turns and pushed back." << endl;
            for (int i = 0; i < 3; ++i)
                moveBack(size, isP1);
        }
        else if (cell == 'G')
        {
            blockTurns = 1;
            cout << name << " encountered Ghost! Blocked for 1 turn." << endl;
        }
        else if (cell == 'L')
        {
            blockTurns = 4;
            cout << name << " encountered Lion! Blocked for 4 turns." << endl;
        }
    }

    void moveBack(int size, bool isP1)
    {
        if (isP1)
        {
            if ((row % 2 == 0 && col - 1 >= 0) || (row % 2 != 0 && col + 1 < size))
            {
                col += (row % 2 == 0) ? -1 : 1;
            }
            else
            {
                row++;
            }
        }
        else
        {
            if ((row % 2 == 0 && col + 1 < size) || (row % 2 != 0 && col - 1 >= 0))
            {
                col += (row % 2 == 0) ? 1 : -1;
            }
            else
            {
                row--;
            }
        }
    }

    void buyAndPlaceHurdle(Board &b)
    {
        cout << "Available options:" << endl
             << "Fire (F): 5 silver" << endl
             << "Snake (S): 3 silver" << endl
             << "Ghost (G): 2 silver" << endl
             << "Lion (L): 5 gold" << endl
             << "Helper Object (H): 2 silver" << endl;
        cout << "You have " << silver << " silver coins, " << gold << " gold coins, and " << helpers << " helper objects." << endl;
        cout << "Enter item to buy: ";
        char h;
        cin >> h;

        if ((h == 'F' && silver >= 5) || (h == 'S' && silver >= 3) || (h == 'G' && silver >= 2) || (h == 'L' && gold >= 5) || (h == 'H' && silver >= 2))
        {
            if (h == 'H')
            {
                helpers++;
                silver -= 2;
                cout << "Bought a helper object." << endl;
                return;
            }

            int r, c;
            cout << "Enter coordinates to place hurdle: ";
            cin >> r >> c;
            b.placeHurdle(r, c, h);
            if (h == 'L')
                gold -= 5;
            else if (h == 'F')
                silver -= 5;
            else if (h == 'S')
                silver -= 3;
            else if (h == 'G')
                silver -= 2;
        }
        else
        {
            cout << "Insufficient coins or invalid item!" << endl;
        }
    }

    bool atCenter(int center)
    {
        return row == center && col == center;
    }
};

void playGame(int boardSize)
{
    Board board(boardSize);
    Player p1("Player 1", boardSize - 1, 0);
    Player p2("Player 2", 0, boardSize - 1);
    bool p1Turn = true;
    int turns = 1;

    while (true)
    {
        cout << "Turn " << turns << ":" << endl
             << endl;
        cout << "Board:" << endl;
        board.displayBoard(p1.getPosition(), p2.getPosition());

        cout << p1.name << "'s turn:" << endl
             << "1. Move" << endl
             << "2. Buy & Place Item" << endl
             << "Choose: ";
        int choice;
        cin >> choice;
        if (choice == 1)
            p1.move(board, true);
        else if (choice == 2)
            p1.buyAndPlaceHurdle(board);
        else
            cout << "Invalid choice." << endl;

        if (p1.atCenter(boardSize / 2))
        {
            cout << p1.name << " reached the center and wins!" << endl;
            break;
        }

        
        system("pause");
        board.displayBoard(p1.getPosition(), p2.getPosition());
        cout << endl;

        cout << p2.name << "'s turn:" << endl
             << "1. Move" << endl
             << "2. Buy & Place Item" << endl
             << "Choose: ";
        cin >> choice;
        if (choice == 1)
            p2.move(board, false);
        else if (choice == 2)
            p2.buyAndPlaceHurdle(board);
        else
            cout << "Invalid choice." << endl;

        if (p2.atCenter(boardSize / 2))
        {
            cout << p2.name << " reached the center and wins!" << endl;
            break;
        }

        turns++;

        system("pause");
        system("cls");
    }
}

int main()
{
    srand(time(0));
    int size;
    cout << "Enter board size (odd number): ";
    cin >> size;
    if (size % 2 == 0)
        size++;
    playGame(size);
    return 0;
}