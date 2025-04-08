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

int main()
{
    Weapon SCAR ("SCAR-L",50,2200);
    Weapon BOW ("Bow",20,1000);
    SCAR.displayinfo();
    BOW.displayinfo();
    return 0;
}
