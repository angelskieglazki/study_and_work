#include <iostream>
#include <vector>
#include <algorithm>

class Observer
{
public:
    virtual void update(int value) = 0;
};

class Subject
{
private:
    std::vector<Observer *> observers;
    int value;

public:
    void attach(Observer *observer) {
        observers.push_back(observer);
    }

    void detach(Observer *observer) {
        std::erase(observers, observer);
    }

    void setValue(int val) {
        value = val;
    }

    int getValue() {
        return value;
    }

    void notify() {
        for (auto observer : observers) {
            observer->update(value);
        }
    }
};

class ConcreteObserver : public Observer {
public:
    void update(int value) override {
        std::cout << "ConcreteObserver:update. New value: " << value << std::endl;
    }
};

int main(int, char **)
{
    Subject subject;
    ConcreteObserver observer1;
    ConcreteObserver observer2;
    subject.attach(&observer1);
    subject.attach(&observer2);
    subject.setValue(10);
    subject.notify();
    subject.setValue(20);
    subject.notify();
    subject.detach(&observer1);
    subject.setValue(30);
    subject.notify();

    std::cout << "Hello, world!\n";
}
