#include <chrono>

class HighResClock
{
public:
    int64_t now()
    {
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch());
        return time.count();
    }
};

#define Clock CrudeTimer::Instance()

class CrudeTimer
{
private:
    HighResClock clock_;
    // Устанавливаем время (в миллисекундах) при создании экземпляра класса
    int64_t startTime_;

    CrudeTimer() { startTime_ = clock_.now(); }

    CrudeTimer(const CrudeTimer &);
    CrudeTimer &operator=(const CrudeTimer &);


public:
    static CrudeTimer *instance();

    // Возвращает, сколько миллисекунд прошло с момента запуска таймера
    int64_t getCurrentTime() { return clock_.now() - startTime_; }
};