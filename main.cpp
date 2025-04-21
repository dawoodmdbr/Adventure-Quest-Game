#include <iostream>
#include <string>
using namespace std;
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
}

int main()
{
    cout << "Hello World!" << endl;
}