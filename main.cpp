#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const int MAX_SIZE = 11;

// Forward declaration
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
    Item *inventory[10];
    int inventoryCount = 0;

public:
    Player(string n, int startX, int startY)
        : name(n), x(startX), y(startY), points(0), gold(10), silver(20) {
        cout << "Player " << name << " created at (" << x << ", " << y << ")\n";
    }

    void move() {
        // Basic right move for now
        if (y + 1 < MAX_SIZE) y++;
        cout << name << " moved to (" << x << ", " << y << ")\n";
    }

    void updatePoints(int p) {
        points += p;
        cout << name << " now has " << points << " points\n";
    }

    void addGold(int g) {
        gold += g;
    }

    void addSilver(int s) {
        silver += s;
    }

    string getName() const { return name; }
    int getX() const { return x; }
    int getY() const { return y; }
};

void coin::interact(Player &player) {
    player.updatePoints(value);
    if (isGold)
        player.addGold(1);
    else
        player.addSilver(1);
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

    void display() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (grid[i][j] == nullptr)
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
    int level;

public:
    AdventureQuest(int size)
        : board(size), currentTurn(1), level(1) {
        player1 = new Player("Player1", 0, 0);
        player2 = new Player("Player2", size - 1, 0);
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
        board.display();
    }

    void playTurn() {
        Player *current = (currentTurn % 2 == 1) ? player1 : player2;
        cout << "\nTurn " << currentTurn << ": " << current->getName() << "'s turn\n";
        current->move();

        Item *item = board.getItemAt(current->getX(), current->getY());
        if (item) {
            item->interact(*current);
            board.removeItemAt(current->getX(), current->getY());
        }

        currentTurn++;

    }

    void runGame(int turns = 10) {
        for (int i = 0; i < turns; i++) {
            displayGameStatus();
            playTurn();
            system("pause"); 
        }
    }

    ~AdventureQuest() {
        delete player1;
        delete player2;
    }
};

int main() {
    AdventureQuest game(5); // 5x5 board
    game.runGame(10);       // Play 10 turns
    return 0;
}
