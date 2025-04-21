#include <iostream>
#include <string>
using namespace std;
int MAX_SIZE = 11;
class Player;
class Item
{
protected:
    string name;
    int x, y;

public:
    Item(string name, int x, int y) : name(name), x(x), y(y) {}
    virtual void interact(Player &player) = 0;
    virtual ~Item() = default;
};
class coin : public Item
{
private:
    int value;
    bool isGold = false;

public:
    coin(string n, int x, int y, int v) : Item(n, x, y), value(v) {}
    void interact(Player &player) override
    {
        player.updatePoints(value);
        if (isGold)
            player.addGold(1);
        else
            player.addSilver(1);
    }
};
class HelperObject : public Item
{
private:
    int uses;

public:
    HelperObject(string n, int x, int y, int u) : Item(n, x, y), uses(u) {}
    void interact(Player &player) override
    {
        cout << "Helper " << name << " used. Uses left: " << --uses << endl;
    }
};
class Hurdle : public Item{
private:
    int penalty;
public:
    Hurdle(string n, int x, int y, int p): Item(n,x,y), penalty(p){}
    void interact(Player &player) override
    {
        cout << "Hurdle " << name << " encountered. Penalty: " << penalty << endl;
        player.updatePoints(-penalty);
    }
};
class Player{
    private:
    string name;
    int points;
    int x, y;
    int gold, silver;
    Item *inventory[10];
    int inventoryCount = 0;
    public:
    
    Player(string n, int x, int y) : name(n), x(x), y(y), points(0), gold(0), silver(0) {
        cout<<"Player " << name << " created at (" << x << ", " << y << ")" << endl;
    }
    void move(){
        cout << "Player moved to (" << x << ", " << y << ")" << endl;
    }
    void pickItem(Item *item){
        if(inventoryCount < 10){
            inventory[inventoryCount++] = item;
            cout << "Picked up " << item->name << endl;
        }else{
            cout << "Inventory full!" << endl;
        }
    }
    void updatePoints(int p){
        points+=p;
        cout << "Points updated. Current points: " << points << endl;
    }
    void addGold(int g){
        gold+=g;
        cout << "Gold updated. Current gold: " << gold << endl;
    }
    void addSilver(int s){
        silver+=s;
        cout << "Silver updated. Current silver: " << silver << endl;
    }
};
class Board{
    int size;
    Item *grid[MAX_SIZE][MAX_SIZE];
    public:
    Board(int s): size(s){
        for(int i=0; i<MAX_SIZE; i++){
            for(int j=0; j<MAX_SIZE; j++){
                grid[i][j] = nullptr;
            }
        }
    }

    int getSize() const {
        return size;
    }

    void placeItem(Item *item, int x, int y){
        if(x>=0 && x<size && y>=0 && y<size){
            if(grid[x][y] == nullptr){
                grid[x][y] = item;
                cout << "Item placed at (" << x << ", " << y << ")" << endl;
            }else{
                cout << "Position already occupied!" << endl;
            }
        }else{
            cout << "Invalid coordinates!" << endl;
        }
    }

    void getItemAt(int x, int y){
        if(x>=0 && x<size && y>=0 && y<size){
            if(grid[x][y] != nullptr){
                cout << "Item at (" << x << ", " << y << "): " << grid[x][y]->name << endl;
            }else{
                cout << "No item at (" << x << ", " << y << ")" << endl;
            }
        }else{
            cout << "Invalid coordinates!" << endl;
        }
    }
    void removeItemAt(int x, int y){
        if(x>=0 && x<size && y>=0 && y<size){
            if(grid[x][y] != nullptr){
                delete grid[x][y];
                grid[x][y] = nullptr;
                cout << "Item removed from (" << x << ", " << y << ")" << endl;
            }else{
                cout << "No item to remove at (" << x << ", " << y << ")" << endl;
            }
        }else{
            cout << "Invalid coordinates!" << endl;
        }
    }

    void display(){
        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                if(grid[i][j]==nullptr)
                    cout<< "_" << " ";
                else
                    cout<< grid[i][j]->name << " ";
                    
            }
            cout << endl;
        }
    }
    
};



int main()
{
    cout << "Hello World!" << endl;
}