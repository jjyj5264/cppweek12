#ifndef SIMULATOR_H
#define SIMULATOR_H

#define RANDOM_SEED 369369

#include <cstdlib>
#include <functional>
#include <queue>
#include <vector>

class Simulator;

class Schedule {
  private:
  double time_;
  std::function<void()> function_; // 이벤트 발생 시 동작할 함수
  // 여기서 동작이라고 함은
  // node, router의 onReceive, 그 이외 모든 service의 동작 등...

  public:
  double time() { return time_; }
  void call() { function_(); }

  Schedule(double time, std::function<void()> function)
      : time_(time), function_(function) {}

  // priority_queue를 사용하기 위해서는 이것이 필요함
  // top(), pop()에서 내부적으로 사용됨
  // 최대 힙을 사용하지만, delay가 작은 것이 상단에 오도록 만들기 위함임
  bool operator<(const Schedule &schedule) const {
    return this->time_ > schedule.time_;
  }
};

class Simulator {
  private:
  static double time_;
  static std::priority_queue<Schedule> schedules_;

  public:
  static double now() { return time_; }
  static void prepare() { srand(RANDOM_SEED); }

  static void schedule(double time, std::function<void()> function) { // schedule(시간, 동작)
    schedules_.push(Schedule(time, function));
  }

  static void run() { // 시뮬레이션 실행
    while (!schedules_.empty()) {
      Schedule schedule = schedules_.top(); // delay가 더 작은 것부터 실행
      schedules_.pop(); // 삭제

      time_ = schedule.time();
      schedule.call();
    }
  }
};

#endif