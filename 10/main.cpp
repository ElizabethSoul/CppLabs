#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <locale>
#include <Windows.h>

using namespace std;

// Базовый класс User
class User {
protected:
    string name;
    int id;
    int accessLevel;

public:
    User(const string& n, int i, int al) : name(n), id(i) {
        if (n.empty()) throw invalid_argument("Имя пользователя не может быть пустым");
        if (al < 0) throw invalid_argument("Уровень доступа не может быть отрицательным");
        accessLevel = al;
    }

    virtual ~User() = default;

    // Геттеры
    string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    // Сеттеры
    void setName(const string& n) {
        if (n.empty()) throw invalid_argument("Имя пользователя не может быть пустым");
        name = n;
    }
    void setAccessLevel(int al) {
        if (al < 0) throw invalid_argument("Уровень доступа не может быть отрицательным");
        accessLevel = al;
    }

    // Виртуальный метод для полиморфизма
    virtual void displayInfo() const = 0;

    // Сериализация для сохранения в файл
    virtual void serialize(ofstream& ofs) const {
        ofs << name << endl << id << endl << accessLevel << endl;
    }

    // Десериализация для загрузки из файла
    virtual void deserialize(ifstream& ifs) {
        getline(ifs, name);
        ifs >> id >> accessLevel;
        ifs.ignore(); // Пропустить символ новой строки
    }
};

// Производный класс Student
class Student : public User {
private:
    string group;

public:
    Student(const string& n, int i, int al, const string& g)
        : User(n, i, al), group(g) {
        if (g.empty()) throw invalid_argument("Группа не может быть пустой");
    }

    ~Student() override = default;

    void displayInfo() const override {
        cout << "Студент: " << name << ", ID: " << id
             << ", Уровень доступа: " << accessLevel
             << ", Группа: " << group << endl;
    }

    void serialize(ofstream& ofs) const override {
        ofs << "Student" << endl;
        User::serialize(ofs);
        ofs << group << endl;
    }

    void deserialize(ifstream& ifs) override {
        User::deserialize(ifs);
        getline(ifs, group);
    }
};

// Производный класс Teacher
class Teacher : public User {
private:
    string department;

public:
    Teacher(const string& n, int i, int al, const string& d)
        : User(n, i, al), department(d) {
        if (d.empty()) throw invalid_argument("Кафедра не может быть пустой");
    }

    ~Teacher() override = default;

    void displayInfo() const override {
        cout << "Преподаватель: " << name << ", ID: " << id
             << ", Уровень доступа: " << accessLevel
             << ", Кафедра: " << department << endl;
    }

    void serialize(ofstream& ofs) const override {
        ofs << "Teacher" << endl;
        User::serialize(ofs);
        ofs << department << endl;
    }

    void deserialize(ifstream& ifs) override {
        User::deserialize(ifs);
        getline(ifs, department);
    }
};

// Производный класс Administrator
class Administrator : public User {
private:
    string role;

public:
    Administrator(const string& n, int i, int al, const string& r)
        : User(n, i, al), role(r) {
        if (r.empty()) throw invalid_argument("Роль не может быть пустой");
    }

    ~Administrator() override = default;

    void displayInfo() const override {
        cout << "Администратор: " << name << ", ID: " << id
             << ", Уровень доступа: " << accessLevel
             << ", Роль: " << role << endl;
    }

    void serialize(ofstream& ofs) const override {
        ofs << "Administrator" << endl;
        User::serialize(ofs);
        ofs << role << endl;
    }

    void deserialize(ifstream& ifs) override {
        User::deserialize(ifs);
        getline(ifs, role);
    }
};

// Класс Resource
class Resource {
private:
    string name;
    int requiredAccessLevel;

public:
    Resource(const string& n, int ral) : name(n), requiredAccessLevel(ral) {
        if (n.empty()) throw invalid_argument("Название ресурса не может быть пустым");
        if (ral < 0) throw invalid_argument("Требуемый уровень доступа не может быть отрицательным");
    }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    string getName() const { return name; }
    int getRequiredAccessLevel() const { return requiredAccessLevel; }

    void serialize(ofstream& ofs) const {
        ofs << name << endl << requiredAccessLevel << endl;
    }

    void deserialize(ifstream& ifs) {
        getline(ifs, name);
        ifs >> requiredAccessLevel;
        ifs.ignore();
    }
};

// Шаблонный класс AccessControlSystem
template<typename T>
class AccessControlSystem {
private:
    vector<User*> users;
    vector<Resource> resources;

    // Собственная реализация поиска по имени
    User* findUserByNameInternal(const string& name) const {
        for (size_t i = 0; i < users.size(); ++i) {
            if (users[i]->getName() == name) {
                return users[i];
            }
        }
        return nullptr;
    }

    // Собственная реализация поиска по ID
    User* findUserByIdInternal(int id) const {
        for (size_t i = 0; i < users.size(); ++i) {
            if (users[i]->getId() == id) {
                return users[i];
            }
        }
        return nullptr;
    }

    // Собственная реализация сортировки пузырьком по уровню доступа
    void bubbleSortUsersByAccessLevel() {
        for (size_t i = 0; i < users.size(); ++i) {
            for (size_t j = 0; j < users.size() - i - 1; ++j) {
                if (users[j]->getAccessLevel() > users[j + 1]->getAccessLevel()) {
                    User* temp = users[j];
                    users[j] = users[j + 1];
                    users[j + 1] = temp;
                }
            }
        }
    }

public:
    ~AccessControlSystem() {
        for (User* user : users) {
            delete user;
        }
    }

    void addUser(User* user) {
        users.push_back(user);
    }

    void addResource(const Resource& resource) {
        resources.push_back(resource);
    }

    void checkAccess(const User& user, const std::string& resourceName) const {
        bool found = false;
        for (const auto& resource : resources) {
            if (resource.getName() == resourceName) { // Сравнение названия ресурса
                found = true;
                if (resource.checkAccess(user)) {
                    std::cout << "Доступ к " << resourceName << " разрешен для " << user.getName() << std::endl;
                } else {
                    std::cout << "Доступ к " << resourceName << " запрещен для " << user.getName() << std::endl;
                }
                break;
            }
        }
        if (!found) {
            std::cerr << "Ресурс \"" << resourceName << "\" не найден. Проверьте правильность ввода.\n";
        }
    }

    void displayAllUsers() const {
        for (const User* user : users) {
            user->displayInfo();
        }
    }

    void displayAllResources() const {
        std::cout << "Доступные ресурсы:\n";
        for (const auto& resource : resources) {
            std::cout << "- " << resource.getName() << " (Требуемый уровень доступа: " << resource.getRequiredAccessLevel() << ")\n";
        }
    }

    void sortUsersByAccessLevel() {
        bubbleSortUsersByAccessLevel();
    }

    User* findUserByName(const string& name) const {
        return findUserByNameInternal(name);
    }

    User* findUserById(int id) const {
        return findUserByIdInternal(id);
    }

    void saveToFile(const string& filename) const {
        ofstream ofs(filename);
        if (!ofs) throw runtime_error("Не удалось открыть файл для записи");

        ofs << users.size() << endl;
        for (const User* user : users) {
            user->serialize(ofs);
        }

        ofs << resources.size() << endl;
        for (const Resource& resource : resources) {
            resource.serialize(ofs);
        }
    }

    void loadFromFile(const string& filename) {
        ifstream ifs(filename);
        if (!ifs) throw runtime_error("Не удалось открыть файл для чтения");

        for (User* user : users) {
            delete user;
        }
        users.clear();
        resources.clear();

        size_t userCount;
        ifs >> userCount;
        ifs.ignore();

        for (size_t i = 0; i < userCount; ++i) {
            string userType;
            getline(ifs, userType);
            User* user = nullptr;
            if (userType == "Student") {
                user = new Student("Temp", 0, 0, "TempGroup");
            } else if (userType == "Teacher") {
                user = new Teacher("Temp", 0, 0, "TempDepartment");
            } else if (userType == "Administrator") {
                user = new Administrator("Temp", 0, 0, "TempRole");
            } else {
                throw runtime_error("Неизвестный тип пользователя");
            }
            user->deserialize(ifs);
            users.push_back(user);
        }

        size_t resourceCount;
        ifs >> resourceCount;
        ifs.ignore();

        for (size_t i = 0; i < resourceCount; ++i) {
            Resource resource("Temp", 0);
            resource.deserialize(ifs);
            resources.push_back(resource);
        }
    }
};

// Вспомогательная функция для добавления пользователя
void addNewUser(AccessControlSystem<void>& system) {
    try {
        cout << "Выберите тип пользователя (1 - Студент, 2 - Преподаватель, 3 - Администратор): ";
        int type;
        cin >> type;
        cin.ignore();

        string name, specific;
        int id, accessLevel;

        cout << "Введите имя (английскими символами): ";
        getline(cin, name);
        cout << "Введите ID: ";
        cin >> id;
        cout << "Введите уровень доступа: ";
        cin >> accessLevel;
        cin.ignore();

        if (type == 1) {
            cout << "Введите группу(английскими символами): ";
            getline(cin, specific);
            system.addUser(new Student(name, id, accessLevel, specific));
        } else if (type == 2) {
            cout << "Введите кафедру(английскими символами): ";
            getline(cin, specific);
            system.addUser(new Teacher(name, id, accessLevel, specific));
        } else if (type == 3) {
            cout << "Введите роль(английскими символами): ";
            getline(cin, specific);
            system.addUser(new Administrator(name, id, accessLevel, specific));
        } else {
            throw invalid_argument("Неверный тип пользователя");
        }
        cout << "Пользователь добавлен успешно!" << endl;
    } catch (const exception& e) {
        cerr << "Ошибка при добавлении пользователя: " << e.what() << endl;
    }
}

// Вспомогательная функция для поиска пользователя
void searchUser(const AccessControlSystem<void>& system) {
    try {
        cout << "Поиск по (1 - Имя, 2 - ID): ";
        int choice;
        cin >> choice;
        cin.ignore();

        User* user = nullptr;
        if (choice == 1) {
            string name;
            cout << "Введите имя: ";
            getline(cin, name);
            user = system.findUserByName(name);
        } else if (choice == 2) {
            int id;
            cout << "Введите ID: ";
            cin >> id;
            user = system.findUserById(id);
        } else {
            throw invalid_argument("Неверный выбор");
        }

        if (user) {
            user->displayInfo();
        } else {
            cout << "Пользователь не найден." << endl;
        }
    } catch (const exception& e) {
        cerr << "Ошибка при поиске: " << e.what() << endl;
    }
}

// Основная функция
int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");
    AccessControlSystem<void> system;

    // Инициализация системы с тестовыми данными
    try {
        system.addUser(new Student("Petrova Elizaveta", 1001, 1, "TRI23"));
        system.addUser(new Teacher("Lyubov Alexandrovna", 2001, 3, "T-University"));
        system.addUser(new Administrator("Besarion Meskhi", 3001, 5, "Rector"));
        system.addResource(Resource("Lecture hall 8-627", 1));
        system.addResource(Resource("Laboratory", 3));
        system.addResource(Resource("Library", 2));
    } catch (const exception& e) {
        cerr << "Ошибка при инициализации: " << e.what() << endl;
        return 1;
    }

    int choice;
    do {
        cout << "\nМеню:\n"<< "1. Добавить пользователя\n"<< "2. Показать всех пользователей\n"<< "3. Сортировать пользователей по уровню доступа\n"
             << "4. Проверить доступ пользователя\n"<< "5. Сохранить данные в файл\n"<< "6. Загрузить данные из файла\n"
             << "7. Поиск пользователя\n"<< "8. Выход\n"<< "Выберите действие: ";
        cin >> choice;
        cin.ignore();

        try {
            switch (choice) {
                case 1:
                    addNewUser(system);
                    break;
                case 2:
                    cout << "Все пользователи:" << endl;
                    system.displayAllUsers();
                    break;
                case 3:
                    system.sortUsersByAccessLevel();
                    cout << "Пользователи отсортированы по уровню доступа." << endl;
                    system.displayAllUsers();
                    break;
                case 4: {
                    std::string name, resourceName;
                    std::cout << "Введите имя пользователя(английскими символами): ";
                    getline(std::cin, name);
                    User* user = system.findUserByName(name);
                    if (user) {
                        system.displayAllResources(); // Вывод списка ресурсов
                        std::cout << "Введите название ресурса: ";
                        getline(std::cin, resourceName);
                        system.checkAccess(*user, resourceName);
                    } else {
                        std::cout << "Пользователь не найден." << std::endl;
                    }
                    break;
                }
                case 5: {
                    string filename;
                    system.saveToFile("info.txt");
                    cout << "Данные сохранены в " << filename << endl;
                    break;
                }
                case 6: {
                    string filename;
                    system.loadFromFile("info.txt");
                    cout << "Данные загружены из " << filename << endl;
                    break;
                }
                case 7:
                    searchUser(system);
                    break;
                case 8:
                    cout << "Программа завершена." << endl;
                    break;
                default:
                    cout << "Неверный выбор. Попробуйте снова." << endl;
            }
        } catch (const exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    } while (choice != 8);

    return 0;
}
