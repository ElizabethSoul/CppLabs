#include <iostream>
#include <string>

class Character {
private:
    std::string name;  // Приватное поле: имя персонажа
    int health;        // Приватное поле: уровень здоровья
    int attack;        // Приватное поле: уровень атаки
    int defense;       // Приватное поле: уровень защиты
    const int max_health = 100; //Максимальное здоровье

public:
    // Конструктор для инициализации данных
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    // Метод для получения уровня здоровья
    int getHealth() const {
        return health;
    }

    // Метод для вывода информации о персонаже
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Метод для атаки другого персонажа
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.health -= damage;
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }

    //Метод для увеличения уровня здоровья персонажа на указанное количество, но не превышающее 100
    void heal(int amount) {
        if (health + amount < max_health)
        {
            health = health + amount;
        }
        else
        {
            std::cout << "The maximum number of HP is exceeded. It was rounded to " << max_health << std::endl;
            health = max_health;
        }
    }

    //Метод для уменьшения здоровья
    void takeDamage(int amount) {
        if (health - amount >= 0)
        {
            health = health - amount;
        }
        else
        {
            health = 0;
        }
    }
};

int main() {
    // Создаем объекты персонажей
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    // Выводим информацию о персонажах
    hero.displayInfo();
    monster.displayInfo();

    // Герой атакует монстра
    hero.attackEnemy(monster);
    monster.displayInfo();

    hero.takeDamage(80);
    hero.displayInfo();
    hero.heal(20);
    hero.displayInfo();
    hero.takeDamage(80);
    hero.displayInfo();
    hero.heal(120);
    hero.displayInfo();
    return 0;
}