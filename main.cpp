#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const int MAX_SIZE = 11;

class Player;

class Item {
protected:
    string itemName;
    int x, y;

public:
    Item(string name, int x, int y) : itemName(name), x(x), y(y) {}
    virtual void interact(Player &player) = 0;
    virtual ~Item() = default;
    string getName() const { return itemName; }
    int getX() const { return x; }
    int getY() const { return y; }
};

class coin : public Item {
private:
    int value;
    bool isGold;

public:
    coin(string n, int x, int y, int v, bool gold)
        : Item(n, x, y), value(v), isGold(gold) {}
    void interact(Player &player) override;
};

class HelperObject : public Item {
private:
    int uses;

public:
    HelperObject(string n, int x, int y, int u)
        : Item(n, x, y), uses(u) {}
    void interact(Player &player) override;
};

class Hurdle : public Item {
private:
    int penalty;

public:
    Hurdle(string n, int x, int y, int p)
        : Item(n, x, y), penalty(p) {}
    void interact(Player &player) override;
};

class Player {
private:
    string name;
    int points;
    int x, y;
    int gold, silver;
    int boardSize;
    bool directionDown;
    bool goalReached = false;

public:
    Player(string n, int startX, int startY, int bSize, bool down)
        : name(n), x(startX), y(startY), points(0), gold(10), silver(20), boardSize(bSize), directionDown(down) {
        cout << "Player " << name << " created at (" << x << ", " << y << ")\n";
    }

    void move() {
        if (goalReached) return;
    
        int rowIdx = directionDown ? (boardSize - 1 - x) : x;
        if (rowIdx % 2 == 0) {
            if ((directionDown && y < boardSize - 1) || (!directionDown && y > 0))
                y += directionDown ? 1 : -1;
            else
                x += directionDown ? -1 : 1;
        } else {
            if ((directionDown && y > 0) || (!directionDown && y < boardSize - 1))
                y += directionDown ? -1 : 1;
            else
                x += directionDown ? -1 : 1;
        }
    
        cout << name << " moved to (" << x << ", " << y << ")\n";
    }

    void updatePoints(int p) {
        points += p;
        cout << name << " now has " << points << " points\n";
    }

    void addGold(int g) { 
        gold += g; 
        cout<< name << "collected a gold coin.\n";
    }
    void addSilver(int s) { 
        silver += s; 
        cout << name << "collected a silver coin.\n";
    }

    string getName() const { return name; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getPoints() const { return points; }
    char getSymbol() const { return toupper(name[0]); }
    bool hasReachedGoal() const { return goalReached; }
};

void coin::interact(Player &player) {
    player.updatePoints(value);
    if (isGold) player.addGold(1);
    else player.addSilver(1);
}

void HelperObject::interact(Player &player) {
    cout << "Helper " << itemName << " used. Uses left: " << --uses << endl;
}

void Hurdle::interact(Player &player) {
    cout << "Hurdle " << itemName << " encountered. Penalty: " << penalty << endl;
    player.updatePoints(-penalty);
}

class Board {
private:
    int size;
    Item *grid[MAX_SIZE][MAX_SIZE];

public:
    Board(int s) : size(s) {
        for (int i = 0; i < MAX_SIZE; i++)
            for (int j = 0; j < MAX_SIZE; j++)
                grid[i][j] = nullptr;
    }

    int getSize() const { return size; }

    void placeItem(Item *item, int x, int y) {
        if (x >= 0 && x < size && y >= 0 && y < size && grid[x][y] == nullptr) {
            grid[x][y] = item;
        }
    }

    void display(Player *p1, Player *p2) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (p1->getX() == i && p1->getY() == j)
                    cout << p1->getSymbol() << " ";
                else if (p2->getX() == i && p2->getY() == j)
                    cout << p2->getSymbol() << " ";
                else if (grid[i][j] == nullptr)
                    cout << "_ ";
                else
                    cout << grid[i][j]->getName()[0] << " ";
            }
            cout << endl;
        }
    }

    Item* getItemAt(int x, int y) {
        if (x >= 0 && x < size && y >= 0 && y < size)
            return grid[x][y];
        return nullptr;
    }

    void removeItemAt(int x, int y) {
        if (x >= 0 && x < size && y >= 0 && y < size) {
            delete grid[x][y];
            grid[x][y] = nullptr;
        }
    }
};

class AdventureQuest {
private:
    Board board;
    Player *player1;
    Player *player2;
    int currentTurn;
    bool gameEnded = false;

public:
    AdventureQuest(int size, const string &p1Name, const string &p2Name)
        : board(size), currentTurn(1) {
        player1 = new Player(p1Name, size - 1, 0, size, true);
        player2 = new Player(p2Name, 0, size - 1, size, false);
        placeInitialItems();
    }

    void placeInitialItems() {
        srand(time(0));
        int numItems = 5 + (rand() % 6);

        for (int i = 0; i < numItems; i++) {
            int x = rand() % board.getSize();
            int y = rand() % board.getSize();
            if (rand() % 2)
                board.placeItem(new coin("Gold", x, y, 10, true), x, y);
            else
                board.placeItem(new coin("Silver", x, y, 5, false), x, y);
        }
    }

    void displayGameStatus() {
        cout << "\n=== Board ===\n";
        board.display(player1, player2);
    }

    void playTurn() {
        if (gameEnded) return;

        Player *current = (currentTurn % 2 == 1) ? player1 : player2;
        cout << "\nTurn " << currentTurn << ": " << current->getName() << "'s turn\n";
        current->move();

        Item *item = board.getItemAt(current->getX(), current->getY());
        if (item) {
            item->interact(*current);
            board.removeItemAt(current->getX(), current->getY());
        }

        currentTurn++;

        if (current->getX() == board.getSize() / 2 && current->getY() == board.getSize() / 2) {
            gameEnded = true;
            cout << "\n=== CENTER REACHED! GAME OVER ===\n";
            cout << current->getName() << " has reached the center and triggered the end!\n";
        
            int p1Points = player1->getPoints();
            int p2Points = player2->getPoints();
        
            cout << player1->getName() << "'s points: " << p1Points << "\n";
            cout << player2->getName() << "'s points: " << p2Points << "\n";
        
            if (p1Points == p2Points)
                cout << "It's a draw!\n";
            else if (p1Points > p2Points)
                cout << player1->getName() << " wins!\n";
            else
                cout << player2->getName() << " wins!\n";
        
            return;
        }
    }

    void runGame(int turns = 100) {
        displayGameStatus();
        system("pause");
        while (!gameEnded) {
            system("cls"); 
            playTurn();
            displayGameStatus();
            system("pause");
        }
    }

    ~AdventureQuest() {
        delete player1;
        delete player2;
    }
};

int main() {
    string name1, name2;
    cout << "Enter Player 1 Name: ";
    getline(cin, name1);
    cout << "Enter Player 2 Name: ";
    getline(cin, name2);

    AdventureQuest game(5, name1, name2);
    game.runGame();

    return 0;
}
