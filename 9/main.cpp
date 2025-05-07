#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <mutex>
#include <random>
#include <locale>
#include <thread>

template<typename T>
class Logger {
private:
    std::ofstream log_file;

    std::string getTimestamp() const {
        std::time_t now = std::time(nullptr);
        std::string time = std::ctime(&now);
        if (!time.empty() && time.back() == '\n') {
            time.pop_back();
        }
        return time;
    }

public:
    Logger(const std::string& filename) {
        log_file.open(filename, std::ios::app);
        if (!log_file.is_open()) {
            throw std::runtime_error("Failed to open log file: " + filename);
        }
    }

    ~Logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void log(const T& message) {
        if (!log_file.is_open()) {
            throw std::runtime_error("Log file is not open");
        }
        log_file << "[" << getTimestamp() << "] " << message << "\n";
        log_file.flush();
    }
};

// Базовый класс для монстров
class Monster {
protected:
    std::string name;
    int health;
    int attack;
    int defense;
    int level;

public:
    Monster(const std::string& n, int h, int a, int d, int l)
        : name(n), health(h), attack(a), defense(d), level(l) {
        std::cout << "Монстр " << name << " появился!\n";
    }

    virtual ~Monster() {
        std::cout << "Монстр " << name << " уничтожен!\n";
    }

    virtual void displayInfo() const {
        std::cout << "Монстр: " << name << "\nЗдоровье: " << health 
                  << "\nАтака: " << attack << "\nЗащита: " << defense 
                  << "\nУровень: " << level << std::endl;
    }

    virtual int calculateDamage() const {
        return attack;
    }

    virtual std::string getType() const = 0;

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    int getLevel() const { return level; }

    void takeDamage(int damage) {
        int actualDamage = std::max(0, damage - defense);
        health -= actualDamage;
        if (health < 0) health = 0;
    }

    bool isAlive() const { return health > 0; }
};

// Производные классы монстров
class Goblin : public Monster {
public:
    Goblin(const std::string& n, int l) 
        : Monster(n, 50 + l*10, 15 + l*2, 5 + l, l) {}

    int calculateDamage() const override {
        return attack;
    }

    void displayInfo() const override {
        std::cout << "Гоблин: " << name << "\nЗдоровье: " << health 
                  << "\nАтака: " << attack << "\nЗащита: " << defense 
                  << "\nУровень: " << level << std::endl;
    }

    std::string getType() const override {
        return "Goblin";
    }
};

class Dragon : public Monster {
public:
    Dragon(const std::string& n, int l) 
        : Monster(n, 100 + l*20, 30 + l*5, 15 + l*2, l) {}

    int calculateDamage() const override {
        return attack;
    }

    void displayInfo() const override {
        std::cout << "Дракон: " << name << "\nЗдоровье: " << health 
                  << "\nАтака: " << attack << "\nЗащита: " << defense 
                  << "\nУровень: " << level << std::endl;
    }

    std::string getType() const override {
        return "Dragon";
    }
};

class Skeleton : public Monster {
public:
    Skeleton(const std::string& n, int l) 
        : Monster(n, 60 + l*15, 20 + l*3, 10 + l, l) {}

    int calculateDamage() const override {
        return attack;
    }

    void displayInfo() const override {
        std::cout << "Скелет: " << name << "\nЗдоровье: " << health 
                  << "\nАтака: " << attack << "\nЗащита: " << defense 
                  << "\nУровень: " << level << std::endl;
    }

    std::string getType() const override {
        return "Skeleton";
    }
};

// Класс инвентаря
class Inventory {
private:
    std::vector<std::string> items;

public:
    Inventory() = default;

    void addItem(const std::string& item) {
        items.push_back(item);
    }

    bool removeItem(const std::string& item) {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i] == item) {
                for (size_t j = i; j < items.size() - 1; ++j) {
                    items[j] = items[j + 1];
                }
                items.pop_back();
                return true;
            }
        }
        return false;
    }

    void displayInventory() const {
        if (items.empty()) {
            std::cout << "Инвентарь пуст" << std::endl;
            return;
        }
        std::cout << "Содержимое инвентаря:" << std::endl;
        for (const auto& item : items) {
            std::cout << "- " << item << std::endl;
        }
    }

    size_t countPotions() const {
        size_t count = 0;
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i] == "Зелье исцеления") {
                ++count;
            }
        }
        return count;
    }
};

// Класс персонажа
class Character {
private:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int level;
    int experience;
    Inventory inventory;

public:
    Character(const std::string& n, int h, int a, int d, int l)
        : name(n), health(h), maxHealth(h), attack(a), defense(d), level(l), experience(0) {
        std::cout << "Персонаж " << name << " создан!\n";
    }

    ~Character() {
        std::cout << "Персонаж " << name << " уничтожен!\n";
    }

    void displayInfo() const {
        std::cout << "Персонаж: " << name << "\nЗдоровье: " << health << "/" << maxHealth
                  << "\nАтака: " << attack << "\nЗащита: " << defense 
                  << "\nУровень: " << level << "\nОпыт: " << experience << std::endl;
        inventory.displayInventory();
    }

    void heal(int amount) {
        health = std::min(maxHealth, health + amount);
        std::cout << name << " восстановил " << amount << " здоровья. Текущее здоровье: " << health << std::endl;
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    int getLevel() const { return level; }
    int getExperience() const { return experience; }

    void takeDamage(int damage) {
        int actualDamage = std::max(0, damage - defense);
        health -= actualDamage;
        if (health < 0) health = 0;
    }

    void gainExperience(int exp) {
        experience += exp;
        while (experience >= level * 100) {
            experience -= level * 100;
            level++;
            maxHealth += 10;
            health = maxHealth;
            attack += 5;
            defense += 2;
            std::cout << name << " повысил уровень до " << level << "!\n";
            std::cout << "Максимальное здоровье: " << maxHealth << ", атака: " << attack 
                      << ", защита: " << defense << ".\n";
        }
    }

    bool isAlive() const { return health > 0; }
    void addItem(const std::string& item) { inventory.addItem(item); }
    bool removeItem(const std::string& item) { return inventory.removeItem(item); }
    size_t countPotions() const { return inventory.countPotions(); }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Не удалось открыть файл для записи");
        }
        file << name << "\n"
             << level << "\n"
             << health << "\n"
             << maxHealth << "\n"
             << attack << "\n"
             << defense << "\n"
             << experience << "\n";
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Не удалось открыть файл для чтения");
        }
        std::getline(file, name);
        file >> level >> health >> maxHealth >> attack >> defense >> experience;
    }
};

// Класс для управления игровыми сущностями
class GameManager {
private:
    std::vector<Monster*> entities;

public:
    ~GameManager() {
        for (auto entity : entities) {
            delete entity; 
        }
    }

    void addEntity(Monster* entity) {
        entities.push_back(entity);
    }

    void displayAll() const {
        if (entities.empty()) {
            std::cout << "Нет монстров\n";
            return;
        }
        for (size_t i = 0; i < entities.size(); ++i) {
            std::cout << i + 1 << ". ";
            entities[i]->displayInfo();
        }
    }

    std::vector<Monster*>& getEntities() {
        return entities;
    }

    void clearEntities() {
        for (auto entity : entities) {
            delete entity;
        }
        entities.clear();
    }
};

// Класс игры
class Game {
private:
    Character* hero;
    GameManager monsterManager;
    std::mutex combatMutex;
    Logger<std::string> logger;

    void combat(Character* hero, Monster* monster) {
        std::cout << "Начинается бой между " << hero->getName() << " и " << monster->getName() << "!\n";
        
        while (hero->isAlive() && monster->isAlive()) {
            std::cout << "\nВаше здоровье: " << hero->getHealth() << "/" << hero->getMaxHealth()
                      << ", Зелья: " << hero->countPotions() << "\n"
                      << "Здоровье монстра: " << monster->getHealth() << "\n"
                      << "1. Атаковать\n2. Использовать зелье исцеления\n";
            int choice;
            std::cin >> choice;
            std::cin.ignore();

            {
                std::lock_guard<std::mutex> lock(combatMutex);

                if (choice == 1) {
                    int heroDamage = hero->getAttack();
                    monster->takeDamage(heroDamage);
                    std::cout << hero->getName() << " наносит " << heroDamage << " урона " << monster->getName() << ". Здоровье монстра: " << monster->getHealth() << std::endl;
                    logger.log(hero->getName() + " атакует " + monster->getName() + " и наносит " + std::to_string(heroDamage) + " урона.");

                } else if (choice == 2) {
                    if (hero->countPotions() > 0) {
                        hero->heal(50);
                        logger.log(hero->getName() + " использует зелье исцеления и восстанавливает 50 здоровья.");
                        hero->removeItem("Зелье исцеления");
                    } else {
                        std::cout << "У вас нет зелий исцеления!\n";
                    }
                } else {
                    std::cout << "Неверный выбор, пропуск хода\n";
                }

                if (monster->isAlive()) {
                    int monsterDamage = monster->calculateDamage();
                    hero->takeDamage(monsterDamage);
                    std::cout << monster->getName() << " наносит " << monsterDamage << " урона " << hero->getName() << ". Здоровье героя: " << hero->getHealth() << std::endl;
                    logger.log(monster->getName() + " атакует " + hero->getName() + " и наносит " + std::to_string(monsterDamage) + " урона.");
                }
            }
        }

        if (hero->isAlive()) {
            std::cout << hero->getName() << " победил в бою!\n";
            hero->gainExperience(monster->getLevel() * 50);
            hero->addItem("Зелье исцеления");
            std::cout << "Получено зелье исцеления!\n";
            logger.log(hero->getName() + " победил " + monster->getName() + " и получил зелье исцеления.");
            hero->addItem("Трофей от " + monster->getName());
        } else {
            std::cout << monster->getName() << " победил в бою!\n";
        }

        if (!hero->isAlive())
        {
            std::cout << hero->getName() << " Вы погибли! Игра окончена\n";
            exit(0);
        }
    }

public:
Game() : hero(nullptr), logger("game_log.txt") {}

    ~Game() {
        delete hero; 
    }

    void start() {
        try {
            std::string name;
            std::cout << "Введите имя персонажа: ";
            std::getline(std::cin, name);
            hero = new Character(name, 100, 20, 10, 1);

            monsterManager.addEntity(new Skeleton("Скелет", hero->getLevel()));
            monsterManager.addEntity(new Dragon("Дракон", hero->getLevel()));
            monsterManager.addEntity(new Goblin("Гоблин", hero->getLevel()));

            while (true) {
                std::cout << "\n1. Показать информацию\n2. Сразиться с монстром\n3. Сохранить игру\n4. Загрузить игру\n5. Выход\n";
                int choice;
                std::cin >> choice;
                std::cin.ignore();

                switch (choice) {
                    case 1:
                        hero->displayInfo();
                        break;
                    case 2: {
                        monsterManager.displayAll();
                        std::cout << "Выберите монстра для боя (1-" << monsterManager.getEntities().size() << "): ";
                        size_t monsterIndex;
                        std::cin >> monsterIndex;
                        std::cin.ignore();
                        if (monsterIndex > 0 && monsterIndex <= monsterManager.getEntities().size()) {
                            Monster* defeatedMonster = monsterManager.getEntities()[monsterIndex - 1];
                            combat(hero, defeatedMonster);
                            std::string defeatedType = defeatedMonster->getType(); 
                            delete defeatedMonster;
                            monsterManager.getEntities().erase(monsterManager.getEntities().begin() + (monsterIndex - 1));

                            Monster* newMonster = nullptr;
                            if (defeatedType == "Goblin") {
                                newMonster = new Goblin("Гоблин", hero->getLevel());
                            } else if (defeatedType == "Dragon") {
                                newMonster = new Dragon("Дракон", hero->getLevel());
                            } else if (defeatedType == "Skeleton") {
                                newMonster = new Skeleton("Скелет", hero->getLevel());
                            }

                            if (newMonster) {
                                monsterManager.addEntity(newMonster);
                            }
                        } else {
                            std::cout << "Неверный выбор монстра\n";
                        }
                        break;
                    }
                    case 3:
                        hero->saveToFile("game_save.txt");
                        std::cout << "Игра сохранена\n";
                        break;
                    case 4:
                        hero->loadFromFile("game_save.txt");
                        std::cout << "Игра загружена\n";
                        break;
                    case 5:
                        return;
                    default:
                        std::cout << "Неверный выбор\n";
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }
};

// Главная функция
int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    Game game;
    game.start();
    return 0;
}