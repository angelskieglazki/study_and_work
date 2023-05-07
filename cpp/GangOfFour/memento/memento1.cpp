#include <iostream>
#include <vector>

class Originator
{
public:
    Originator() : state_(0) {}

    void setState(int state)
    {
        std::cout << "Originator: Setting state to " << state << '\n';
        state_ = state;
    }

    int getState() const
    {
        return state_;
    }

    class Memento
    {
    public:
        explicit Memento(int state) : state_(state) {}
        int getState() const
        {
            return state_;
        }

    private:
        int state_;
    };

    Memento createMemento() const
    {
        std::cout << "Originator: Creating Memento\n";
        return Memento(state_);
    }

    void restoreMemento(const Memento &memento)
    {
        std::cout << "Originator: Restoring state to " << memento.getState() << '\n';
        state_ = memento.getState();
    }

private:
    int state_;
};

class CareTaker
{
public:
    CareTaker() : current_memento_index_(-1) {}

    void addMemento(const Originator::Memento &memento)
    {
        mementos_.push_back(memento);
        current_memento_index_ = mementos_.size() - 1;
        std::cout << "CareTaker: Added Memento\n";
    }

    Originator::Memento getCurrentMemento() const
    {
        if (mementos_.empty())
        {
            return Originator::Memento(0);
        }
        return mementos_[mementos_.size() - 1];
    }

    void undo() {
        if (current_memento_index_ < 0) {
            return;
        }

        std::cout << "CareTaker: Undoing\n";
        current_memento_index_--;
    }

    void restore() {
        if (current_memento_index_ < 0 || current_memento_index_ >= mementos_.size()) {
            return;
        }

        std::cout << "CareTaker: Restoring\n";
        const auto& memento = mementos_[current_memento_index_];
        originator_->restoreMemento(memento);
        current_memento_index_--;
    }

    void setOriginator(Originator *originator) {
        originator_ = originator;
    }
private:
    std::vector<Originator::Memento> mementos_;
    int current_memento_index_;
    Originator* originator_;
};

int main(int, char **)
{
    Originator originator;
    CareTaker caretaker;

    // Устанавливаем объект-оригинатор для хранения снимков
    caretaker.setOriginator(&originator);

    // Изменяем состояние объекта и сохраняем снимок
    originator.setState(1);
    caretaker.addMemento(originator.createMemento());

    // Изменяем состояние объекта и сохраняем снимок
    originator.setState(2);
    caretaker.addMemento(originator.createMemento());

    // Изменяем состояние объекта и сохраняем снимок
    originator.setState(3);
    caretaker.addMemento(originator.createMemento());

    // Отменяем последнее действие и восстанавливаем состояние
    caretaker.undo();
    caretaker.restore();
}
