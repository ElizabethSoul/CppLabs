#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Character " << name << " created!\n";
    }

    // Деструктор
    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
    // Перегрузка оператора ==
    bool operator==(const Character& other) const {
        return name == other.name && health == other.health;
    }

    // Перегрузка оператора <<
    friend std::ostream& operator<<(std::ostream& os, const Character& character) {
        os << "Character: " << character.name << ", HP: " << character.health
           << ", Attack: " << character.attack << ", Defense: " << character.defense;
        return os;
    }
};

class Weapon {
private:
    std::string name;
    int damage;
    int weight;
public:
    Weapon(const std::string& n, int d, int w)
        : name(n), damage(d), weight(w) {
        std::cout << "Weapon " << name << " created!" << std::endl;
    }

    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!" << std::endl;
    }

    void displayinfo() const {
        std::cout << "Weapon info: " << std::endl;
        std::cout << "Name: " << name << " damage: " << damage << " weight: " << weight << std::endl;
    }

    bool operator==(const Weapon& other) const {
        return damage == other.damage;
    }

    Weapon operator+(const Weapon& other) const{
        std::string newName = this->name + " + " + other.name;
        int attack = this->damage + other.damage;
        int weight = this->weight + other.weight;
        return Weapon(newName, attack, weight);
    }
    std::string getName() const{
        return name;
    }

};
class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created!\n";
    }

    // Деструктор
    ~Monster() {
        std::cout << "Monster " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Inventory{
    private:
    std::string* items;
    size_t capacity;
    size_t count;
    public:
    Inventory(size_t initialCapacity = 10) : capacity(initialCapacity), count(0) {
        items = new std::string[capacity];
    }
    ~Inventory() {
        delete[] items;
    }
    void addItem(const std::string& item) {
        if (count >= capacity) {
            size_t newCapacity = capacity * 2;
            std::string* newItems = new std::string[newCapacity];

            for (size_t i = 0; i < count; ++i) {
                newItems[i] = std::move(items[i]);
            }

            delete[] items;
            items = newItems;
            capacity = newCapacity;
        }
        items[count] = item;
        ++count;
    }
    void displayInventory() const {
        if (count == 0) {
            std::cout << "No items in inventry" << std::endl;
            return;
        }
        std::cout << "Inventory contents:" << std::endl;
        for (size_t i = 0; i < count; ++i) {
            std::cout << "- " << items[i] << std::endl;
        }
    }

};

int main()
{
    std::unique_ptr<Inventory>  inventory[] = {
        std::make_unique<Inventory>(3),
        std::make_unique<Inventory>(2)
    };
    inventory[0]->addItem("Weapon");
    inventory[0]->addItem("Knife");
    inventory[0]->addItem("Heal posion");
    inventory[0]->addItem("Sheld");


    inventory[1]->addItem("Gloves");
    inventory[1]->addItem("Helmet");
    for (const auto& inv : inventory) {
        inv->displayInventory();
    }
    return 0;
}
