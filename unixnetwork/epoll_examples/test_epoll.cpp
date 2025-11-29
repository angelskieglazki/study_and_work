#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>

constexpr int PORT = 8080;
constexpr int NUM_CLIENTS = 1000;

void client_job(int id) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    throw std::runtime_error("socket");
    return;
  }

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

  if (connect(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
    throw std::runtime_error("connect");
    close(sock);
    return;
  }

  fcntl(sock, F_SETFL, O_NONBLOCK);

  std::string message = "client_" + std::to_string(id) + ": Hello epoll!\n";
  const char *ptr = message.c_str();
  size_t left = message.size();

  while (left > 0) {
    ssize_t n = write(sock, ptr, left);
    if (n <= 0) {
      if (errno == EAGAIN) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        continue;
      }
      perror("write");
      break;
    }
    ptr += n;
    left -= n;
  }

  std::string received;
  char buf[128];
  while (true) {
    ssize_t n = read(sock, buf, sizeof(buf));
    if (n <= 0) {
      if (n == 0) {
        break;
      }
      if (errno == EAGAIN) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        continue;
      }
      perror("read");
      break;
    }
    received.append(buf, n);
    if (received.back() == '\n') {
      break;
    }
  }

  close(sock);

  if (received == message) {

  } else {
    std::cerr << "Received unexpected message: " << received
              << ", expected: " << message << "\n";
    exit(1);
  }
}

int main() {
  pid_t pid = fork();

  if (pid == 0) {
    execl("./epoll_server", "epoll_server", (char *)nullptr);
    perror("execl failed");
    exit(1);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  std::cout << "Run " << NUM_CLIENTS << " clients...\n";

  auto start = std::chrono::steady_clock::now();

  std::vector<std::thread> threads;
  for (int i = 0; i < NUM_CLIENTS; ++i) {
    threads.emplace_back(client_job, i);
  }

  for (auto &t : threads)
    t.join();

  auto end = std::chrono::steady_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();

  std::cout << "All " << NUM_CLIENTS << " clients finished in " << ms
            << " ms\n";

  // Убиваем сервер
  kill(pid, SIGTERM);
  waitpid(pid, nullptr, 0);

  std::cout << "All done!\n";
  return 0;
}