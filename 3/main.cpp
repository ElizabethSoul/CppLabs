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

int main()
{
    Character hero1("Hero", 100, 20, 10);
    Character hero2("Hero", 100, 20, 10);
    Character hero3("Warrior", 150, 25, 15);
    Weapon SCAR_L ("SCAR-L",50,2500);
    Weapon Bow ("Bow",45,500);
    Weapon AWP ("AWP",95,3000);
    if (hero1 == hero2)
    {
        std::cout << "Hero1 and Hero2 are the same!\n";
    }
    if (!(hero1 == hero3))
    {
        std::cout << "Hero1 and Hero3 are different!\n";
    }
    std::cout << hero1 << std::endl; // Вывод информации о персонаже
    Weapon NewWeapon = SCAR_L + Bow;
    NewWeapon.displayinfo();
    if (!(NewWeapon == SCAR_L))
    {
        std::cout<<"Damage of " <<NewWeapon.getName() << " and " << SCAR_L.getName() << " are different!\n";
    }
    else{
        std::cout<<"Damage of " <<NewWeapon.getName() << " and " << SCAR_L.getName() << " are the same!\n";
    }
    if (!(NewWeapon == AWP))
    {
        std::cout<<"Damage of " <<NewWeapon.getName() << " and " << AWP.getName() << " are different!\n";
    }
    else{
        std::cout<<"Damage of " <<NewWeapon.getName() << " and " << AWP.getName() << " are the same!\n";
    }
    return 0;
}
