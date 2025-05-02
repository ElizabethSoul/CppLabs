#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

template <typename T>
class Queue {
private:
    std::vector<T> contents;
public:
    void push(const T& content) {
        contents.push_back(content);
    }
    
    T pop() {
        if (contents.empty()) {
            throw std::runtime_error("Cannot pop from empty queue");
        }
        T frontItem = contents.front();
        contents.erase(contents.begin());
        return frontItem;
    }
    
    void display() const {
        for (const auto &content : contents) {
            std::cout << content << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    // Test string queue
    Queue<std::string> stringQueue;
    stringQueue.push("Weapon");
    stringQueue.push("Knife");
    stringQueue.push("Patron");

    std::cout << "Initial string queue: ";
    stringQueue.display();
    
    try {
        std::cout << "Popped: " << stringQueue.pop() << std::endl;
        std::cout << "After pop: ";
        stringQueue.display();
        
        // Attempt to pop more times than elements exist
        stringQueue.pop();
        stringQueue.pop();
        stringQueue.pop(); // This should throw an exception
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Test integer queue
    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.push(30);

    std::cout << "\nInitial integer queue: ";
    intQueue.display();
    
    try {
        std::cout << "Popped: " << intQueue.pop() << std::endl;
        std::cout << "After pop: ";
        intQueue.display();
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
