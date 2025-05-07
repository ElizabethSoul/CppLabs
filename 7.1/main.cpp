#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream> // Добавлено для работы с файлами

class Character {
private:
    std::string name;
    int health;
    int level;

public:
    // Конструктор
    Character(const std::string& n, int h, int l)
        : name(n), health(h), level(l) {
        std::cout << "Character " << name << " created!\n";
    }

    // Деструктор
    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << "\nHP: " << health << "\nLevel: " << level << std::endl;
    }

    std::string GetName() const { return name; }
    int GetLevel() const { return level; }
    int GetHealth() const { return health; }
};

template <typename T>
class GameManager {
private:
    std::vector<T*> entities;

public:

    void addEntity(T* entity) {
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto* entity : entities) {
            entity->displayInfo();
        }
    }


    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for writing.");
        }
        for (const auto* entity : entities) {
            file << entity->GetName() << "\n" 
                 << entity->GetLevel() << "\n" 
                 << entity->GetHealth() << "\n";
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for reading.");
        }

        std::string name;
        int health, level;
        while (file >> name >> level >> health) {
            entities.push_back(new Character(name, health, level));
        }
    }

    ~GameManager() {
        for (auto* entity : entities) {
            delete entity;
        }
    }
};

int main() {
    try {

        GameManager<Character> manager;
        manager.addEntity(new Character("Akita", 100, 5));
        manager.addEntity(new Character("Akira", 80, 3));

        std::cout << "Characters created!:\n";
        manager.displayAll();


        manager.saveToFile("game_save.txt");


        GameManager<Character> loadedManager;
        loadedManager.loadFromFile("game_save.txt");

        std::cout << "\nLoaded characters:\n";
        loadedManager.displayAll();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}