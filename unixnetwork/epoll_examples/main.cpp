#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

class FD {
public:
  explicit FD(int fd = -1) : fd_(fd) {}
  FD(const FD &) = delete;
  FD &operator=(const FD &) = delete;
  FD(FD &&other) noexcept : fd_(other.release()) {}
  FD &operator=(FD &&other) noexcept {
    if (this != &other) {
      reset(other.release());
    }
    return *this;
  }

  ~FD() { close(); }
  void close() {
    if (fd_ != -1) {
      ::close(fd_);
      fd_ = -1;
    }
  }
  int get() const { return fd_; }
  int release() {
    int fd = fd_;
    fd_ = -1;
    return fd;
  }
  void reset(int fd = -1) {
    if (fd_ != -1) {
      ::close(fd_);
    }
    fd_ = fd;
  }

private:
  int fd_ = -1;
};

class Epoll {
public:
  Epoll() {
    int fd = ::epoll_create1(0);
    if (fd < 0) {
      throw std::runtime_error("epoll_create1");
    }
    epoll_fd_.reset(fd);
  }

  void add(int fd, uint32_t events, void *ptr = nullptr) {
    epoll_event ev{};
    ev.events = events;
    ev.data.ptr = ptr;
    if (epoll_ctl(epoll_fd_.get(), EPOLL_CTL_ADD, fd, &ev) < 0) {
      throw std::runtime_error("epoll_ctl ADD: " +
                               std::string(strerror(errno)));
    }
  }

  void modify(int fd, uint32_t events, void *ptr = nullptr) {
    epoll_event ev{};
    ev.events = events;
    ev.data.ptr = ptr;
    if (epoll_ctl(epoll_fd_.get(), EPOLL_CTL_MOD, fd, &ev) < 0) {
      throw std::runtime_error("epoll_ctl MODIFY: " +
                               std::string(strerror(errno)));
    }
  }

  void del(int fd) { epoll_ctl(epoll_fd_.get(), EPOLL_CTL_DEL, fd, nullptr); }

  int wait(epoll_event *events, int max_events, int timeout = -1) {
    int n = epoll_wait(epoll_fd_.get(), events, max_events, timeout);
    if (n < 0) {
      throw std::runtime_error("epoll_wait: " + std::string(strerror(errno)));
    }
    return n;
  }

private:
  FD epoll_fd_;
};

void set_nonblocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) {
    throw std::runtime_error("fcntl: " + std::string(strerror(errno)));
  }
  flags |= O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flags) < 0) {
    throw std::runtime_error("fcntl: " + std::string(strerror(errno)));
  }
}

std::string client_addr(int fd) {
  sockaddr_in addr{};
  socklen_t len = sizeof(addr);
  if (getpeername(fd, reinterpret_cast<sockaddr *>(&addr), &len) < 0) {
    throw std::runtime_error("getpeername: " + std::string(strerror(errno)));
  }
  char ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
  return std::string(ip) + ":" + std::to_string(ntohs(addr.sin_port));
}

class EchoServer {
public:
  void run() {
    int fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (fd == -1) {
      throw std::runtime_error("socket");
    }

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
      throw std::runtime_error("bind: " + std::string(strerror(errno)));
    }

    if (listen(fd, SOMAXCONN) < 0) {
      throw std::runtime_error("listen: " + std::string(strerror(errno)));
    }

    listen_fd_.reset(fd);
    epoll_.add(fd, EPOLLIN, (void*)(uintptr_t)fd);
    std::cout << "ECHO-server started on port: " << PORT << "\n";

    std::vector<epoll_event> events(MAX_EVENTS);

    while (true) {
      int n = epoll_.wait(events.data(), MAX_EVENTS);

      for (int i = 0; i < n; ++i) {
        int client_fd = (uintptr_t)events[i].data.ptr;
        if (client_fd == listen_fd_.get()) {
          handle_accept();
        } else {
          if (events[i].events & (EPOLLHUP | EPOLLRDHUP | EPOLLERR)) {
            close_client(client_fd);
          } else {
            if (events[i].events & EPOLLIN) {
              handle_read(client_fd);
            }
            if (events[i].events & EPOLLOUT) {
              handle_write(client_fd);
            }
          }
        }
      }
    }
  }

private:
  void handle_accept() {
    while (true) {
      int client_fd =
          accept4(listen_fd_.get(), nullptr, nullptr, SOCK_NONBLOCK);
      if (client_fd == -1) {
        if (errno == EAGAIN) {
          break;
        }
        throw std::runtime_error("accept4: " + std::string(strerror(errno)));
      }

      try {
        epoll_.add(client_fd, EPOLLIN | EPOLLET | EPOLLRDHUP,
                   (void *)(uintptr_t)client_fd);
        std::cout << "Client connected: " << client_addr(client_fd) << "\n";
      } catch (...) {
        close(client_fd);
        throw;
      }
    }
  }

  void handle_read(int fd) {
    char buffer[BUFFER_SIZE];

    while (true) {
      ssize_t n = read(fd, buffer, BUFFER_SIZE);
      if (n <= 0) {
        if (n == 0 || (n == -1 && errno != EAGAIN)) {
          close_client(fd);
        }
        return;
      }

      ssize_t written = 0;
      while (written < n) {
        ssize_t res = write(fd, buffer + written, n - written);
        if (res <= 0) {
          if (errno == EAGAIN) {
            auto &buf = client_buffers_[fd];
            buf.insert(buf.end(), buffer + written, buffer + n);
            epoll_.modify(fd, EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP,
                          (void *)(uintptr_t)fd);
          } else {
            close_client(fd);
          }
          return;
        }
        written += res;
      }
    }
  }

  void handle_write(int fd) {
    auto it = client_buffers_.find(fd);
    if (it == client_buffers_.end()) {
      return;
    }

    auto &buf = it->second;
    while (!buf.empty()) {
      ssize_t n = write(fd, buf.data(), buf.size());
      if (n <= 0) {
        if (n == -1 && errno == EAGAIN) {
          break;
        }
        close_client(fd);
        return;
      }
      buf.erase(buf.begin(), buf.begin() + n);
    }

    if (buf.empty()) {
      client_buffers_.erase(it);
      epoll_.modify(fd, EPOLLIN | EPOLLET | EPOLLRDHUP, (void *)(uintptr_t)fd);
    }
  }

  void close_client(int fd) {
    std::cout << "Client disconnected: " << client_addr(fd) << "\n";
    epoll_.del(fd);
    close(fd);
    client_buffers_.erase(fd);
  }

private:
  static constexpr int PORT = 8080;
  static constexpr int MAX_EVENTS = 1024;
  static constexpr int BUFFER_SIZE = 1024;

  FD listen_fd_;
  Epoll epoll_;
  std::unordered_map<int, std::vector<char>> client_buffers_;
};

int main() {
  try {
    EchoServer server;
    server.run();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
