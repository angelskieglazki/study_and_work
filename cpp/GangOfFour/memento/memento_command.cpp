#include <vector>
#include <iostream>
#include <string>
#include <memory>

class ICommand
{
public:
  virtual void execute() = 0;
  virtual void undo() = 0;
};

class Snapshot
{
private:
  std::string data;

public:
  Snapshot(const std::string &data) : data(data) {}
  std::string getData() const { return data; }
};

class Receiver
{
private:
  std::string data;
  std::vector<std::shared_ptr<Snapshot>> snapshots;

public:
  void setData(const std::string &str) { data = str; }
  std::string getData() const { return data; }

  void createSnapshot() { snapshots.push_back(std::make_shared<Snapshot>(data)); }

  void undo()
  {
    if (snapshots.size() > 0)
    {
      snapshots.pop_back();
      data = snapshots.back()->getData();
    }
  }

  void executeCommand(ICommand *command)
  {
    command->execute();
    createSnapshot();
  }
};

class ChangeDataCmd : public ICommand
{
private:
  std::string newData;
  Receiver &receiver;

public:
  ChangeDataCmd(Receiver &receiver, const std::string &data) : receiver(receiver), newData(data) {}

  void execute() override { receiver.setData(newData); }
  void undo() override { receiver.undo(); }
};

int main()
{
  Receiver receiver;
  receiver.setData("hello");
  receiver.createSnapshot();
  std::shared_ptr<ICommand> command = std::make_shared<ChangeDataCmd>(receiver, "World");
  receiver.executeCommand(command.get());

  std::cout << receiver.getData() << std::endl;

  receiver.undo();

  std::cout << receiver.getData() << std::endl;
  return 0;
}