#include <string>
#include <ctime>
#include <iostream>
#include <vector>

class Memento
{
public:
  virtual std::string getName() const = 0;
  virtual std::string date() const = 0;
  virtual std::string state() const = 0;
};

class ConcreteMemento : public Memento
{
private:
  std::string state_;
  std::string date_;

public:
  ConcreteMemento(std::string state) : state_(state)
  {
    std::time_t now = std::time(0);
    date_ = std::asctime(std::localtime(&now));
  }

  std::string state() const override { return state_; }
  std::string getName() const override { return date_ + " / (" + state_.substr(0, 9) + "...)"; }
  std::string date() const override { return date_; }
};

class Originator
{
private:
  std::string state_;

  std::string generateRandomString(int len = 10)
  {
    const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string str;
    for (int i = 0; i < len; ++i)
      str += alphanum[rand() % (sizeof(alphanum) - 1)];
    return str;
  }

public:
  Originator(std::string state) : state_(state)
  {
    std::cout << "Originator: My initial state is " << state_ << std::endl;
  }

  void doSomething()
  {
    std::cout << "Originator: I'm doing something important." << std::endl;
    state_ = generateRandomString(30);
    std::cout << "Originator: and my state has changed to " << state_ << std::endl;
  }

  Memento *save() { return new ConcreteMemento(state_); }

  void restore(Memento *m)
  {
    state_ = m->getName();
    std::cout << "Originator: My state has changed to " << state_ << std::endl;
  }
};

class CareTaker
{
private:
  std::vector<Memento *> mementos_;
  Originator *originator_;

public:
  CareTaker(Originator *originator) : originator_(originator) { }

  ~CareTaker() {
    for (auto m : mementos_)
      delete m;
  }

  void backup() {
    std::cout << "CareTaker: Saving Originator's state..." << std::endl;
    mementos_.push_back(originator_->save());
  }

  void undo() {
    if (mementos_.empty()) return;

    Memento *m = mementos_.back();
    mementos_.pop_back();
    std::cout << "CareTaker: Restoring state to " << m->getName() << std::endl;
    try {
      originator_->restore(m);
    } catch (...) {
      undo();
    }
  }

  void showHistory() {
    std::cout<< "CareTaker: Here's the list of mementos:" << std::endl;
    for (auto& m : mementos_) {
      std::cout << m->getName() << std::endl;
    }
  }
};

int main() {
  std::srand(static_cast<unsigned int>(std::time(0)));
  Originator *originator = new Originator("State 1");
  CareTaker *careTaker = new CareTaker(originator);
  careTaker->backup();
  originator->doSomething();
  careTaker->backup();
  originator->doSomething();
  careTaker->backup();
  originator->doSomething();
  std::cout<< '\n';
  careTaker->showHistory();
  std::cout << "\nClient: Now, let's rollback!\n\n";
  careTaker->undo();
  std::cout << "\nClient: Once more!\n\n";
  careTaker->undo();

  delete originator;
  delete careTaker;
}