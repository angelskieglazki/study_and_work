#include <iostream>
#include <chrono>
#include <deque>
#include <thread>
#include <mutex>
#include <random>
#include <atomic>
#include <queue>

constexpr double K = 1.0; // Коэффициент уменьшения
constexpr double MAX_COUNTER = 20.0; // Максимально допустимый счётчик
constexpr double MIN_COUNTER = 0.0; // Минимальное значение счётчика
constexpr double LINEAR_DECREASE = 1.0; // Линейное уменьшение на шаг

constexpr int MAX_PENALTIES = 3; // Максимальное количество штрафов

// Структура для хранения времени поступления команд
struct Command {
  int id = 0;
};

// Структура игрока
struct Player {
  int counter = 0.0; // Счётчик команд
  int penalties = 0; // Количество штрафов
  std::chrono::steady_clock::time_point
      lastPenaltyTime; // Время последнего штрафа
  std::chrono::steady_clock::time_point
      lastCounterDecrease; // Время последнего уменьшения
  std::queue<Command> commandQueue; // Очередь команд
  std::mutex playerMutex; // Мьютекс для синхронизации доступа
};

// Функция обнуления счётчика, если он ниже минимального значения
double zeroClamp(double counter) { return (counter < 1.0) ? 0.0 : counter; }

// Функция уменьшения счётчика с использованием квадратного корня
double decreaseCounterWithSqrt(double counter) {
  double decreaseValue = K * std::sqrt(counter);
  counter = std::max(counter - decreaseValue, MIN_COUNTER);
  return zeroClamp(counter);
}

// // Функция уменьшения счётчика с использованием логарифма
// double decreaseCounterWithLog(double counter) {
//   double decreaseValue =
//       K * std::log(counter + 1); // +1, чтобы логарифм был определён для 0
//   counter = std::max(counter - decreaseValue, MIN_COUNTER);
//   return zeroClamp(counter);
// }

// Функция для обработки команд с учётом задержек
double processCommands(Player &player) {
  std::lock_guard<std::mutex> lock(player.playerMutex);

  auto now = std::chrono::steady_clock::now();

  int receied_commands = 0;
  while (!player.commandQueue.empty()) {
    auto &command = player.commandQueue.front();
    std::cout << "\tОбработка команды: " << command.id << "\n";
    player.counter++;
    receied_commands++;
    player.commandQueue.pop();
  }

  if (std::chrono::duration_cast<std::chrono::milliseconds>(
          now - player.lastCounterDecrease)
          .count() > 500) {
    // Плавно уменьшаем счётчик каждую секунду
    player.counter = std::max(player.counter - LINEAR_DECREASE, MIN_COUNTER);
    player.lastCounterDecrease = now;

    std::cout << "Счётчик обновлен на: " << player.counter << "\n";
  }
  if (receied_commands != 0) {
    std::cout << "Получено команд: " << receied_commands
              << ", текущий счётчик: " << player.counter << "\n";
  }

  // Проверка превышения лимита
  if (player.counter > MAX_COUNTER) {
    // Назначаем штраф
    player.penalties++;
    player.lastPenaltyTime = now; // Обновляем время последнего штрафа
    std::cout << "Штраф назначен! Текущий счётчик штрафов: " << player.penalties
              << "\n";
    // Откатили счётчик побольше, вдруг просто всплеск
    player.counter = decreaseCounterWithSqrt(player.counter);
    // Если штрафов больше 3, кикаем игрока
    if (player.penalties >= MAX_PENALTIES) {
      std::cout << "Игрок кикнут! Превышено количество штрафов.\n";
      return -1; // Кик игрока
    }
  }

  // Если прошло больше 10 секунд, сбрасываем штрафы
  if (std::chrono::duration_cast<std::chrono::seconds>(now -
                                                       player.lastPenaltyTime)
          .count() > 10) {
    player.penalties = 0;
    std::cout << "Штрафы сброшены.\n";
    player.lastPenaltyTime = now;
  }

  return player.counter;
}

// Функция, которая будет генерировать случайные команды для игрока
void generateCommands(Player &player, std::atomic<bool> &running) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 3);

  int id_cmd = 0;
  while (running) {
    int numCommands = dis(gen); // Случайное количество команд

    std::queue<Command> c;
    for (int j = 0; j < numCommands; ++j) {
      c.push({++id_cmd});
    }

    {
      std::lock_guard<std::mutex> lock(player.playerMutex);
      player.commandQueue.swap(c);
    }

    // Задержка перед следующей генерацией команд
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

// Вечный цикл обработки команд
void processCommandsForever(Player &player, std::atomic<bool> &running) {
  while (running) {
    int counter = processCommands(player); // Обработка команд

    if (counter == -1) {
      running = false;
    }
    std::this_thread::sleep_for(
        std::chrono::milliseconds(100)); // Пауза для имитации обработки
  }
}

int main() {
  auto now = std::chrono::steady_clock::now();
  Player player;
  player.lastPenaltyTime = now; // Инициализация игрока
  player.lastCounterDecrease = now;
  std::atomic<bool> running(true); // Флаг для остановки потоков

  std::cout << "Симуляция с многопоточностью и случайными командами:\n";

  // Запускаем два потока: один для генерации команд, второй для их обработки
  std::thread generatorThread(generateCommands, std::ref(player),
                               std::ref(running));
  std::thread processorThread(processCommandsForever, std::ref(player),
                               std::ref(running));

  processorThread.join();
  generatorThread.join();

  std::cout << "Все потоки завершены.\n";
  return 0;
}



