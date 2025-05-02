#include <iostream>
#include <string>
#include <vector>

template <typename T>
class Queue{
    private:
    std::vector<T> contents;
    public:
    void push(const T& content) {
        contents.push_back(content);
    }
    T pop() {
        T frontItem = contents.front();
        contents.erase(contents.begin());
        return frontItem;
    }
    void display() const
    {
        for (const auto &content : contents)
        {
            std::cout << content << " ";
        }
        std::cout << std::endl;
    }
};

int main()
{
    Queue<std::string> stringQueue;
    stringQueue.push("Weapon");
    stringQueue.push("Knife");
    stringQueue.push("Patron");

    std::cout << "Initial queue: ";
    stringQueue.display();
    std::cout << "Popped: " << stringQueue.pop() << std::endl;
    std::cout << "After pop: ";
    stringQueue.display();


    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.push(30);

    std::cout << "Initial queue: ";
    intQueue.display();
    std::cout << "Popped: " << intQueue.pop() << std::endl;
    std::cout << "After pop: ";
    intQueue.display();
    return 0;
}
