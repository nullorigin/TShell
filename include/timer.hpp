#ifndef TIMER_HPP
#define TIMER_HPP
#include <chrono>
#include <ctime>
#include <string>
namespace origin {
using namespace std::chrono;
static int TimerNumber;
struct Timer {
private:
  nanoseconds TimeStarted = nanoseconds::zero();
  nanoseconds TimeResumed = nanoseconds::zero();
  nanoseconds TimePaused = nanoseconds::zero();
  nanoseconds TimeStopped = nanoseconds::zero();
  nanoseconds TimeRestarted = nanoseconds::zero();
  nanoseconds TimeLimit = nanoseconds::zero();
  nanoseconds TimeOffset = nanoseconds::zero();
  std::string TimerName;

public:
  const int None = 0;
  const int Started = 1;
  const int Paused = 2;
  const int Resumed = 3;
  const int Stopped = 4;
  const int Restarted = 5;
  int State = None;
  Timer() {
    TimeStarted = nanoseconds::zero();
    TimePaused = nanoseconds::zero();
    TimeResumed = nanoseconds::zero();
    TimeStopped = nanoseconds::zero();
    TimeRestarted = nanoseconds::zero();
    TimeOffset = nanoseconds::zero();
    TimeLimit = seconds(100000);
    TimerNumber = TimerNumber + 1;
    TimerName = "Timer_" + std::to_string(TimerNumber) + "]-(Timer" +
                std::to_string(TimerNumber) + ")";
  }
  Timer(std::string name) {
    TimeStarted = nanoseconds::zero();
    TimePaused = nanoseconds::zero();
    TimeResumed = nanoseconds::zero();
    TimeStopped = nanoseconds::zero();
    TimeRestarted = nanoseconds::zero();
    TimeOffset = nanoseconds::zero();
    TimeLimit = seconds(100000);
    TimerNumber = TimerNumber + 1;
    TimerName = name;
    TimerName = "Timer [" + std::to_string(TimerNumber) + "]-(" + name + ")";
  }
  ~Timer() {
    State = None;
    TimeResumed = nanoseconds::zero();
    TimePaused = nanoseconds::zero();
    TimeStarted = nanoseconds::zero();
    TimeStopped = nanoseconds::zero();
    TimeRestarted = nanoseconds::zero();
    TimeOffset = nanoseconds::zero();
    TimeLimit = nanoseconds::zero();
    TimerName = "";
    TimerNumber = TimerNumber - 1;
  }
  auto operator=(const Timer & /*unused*/) -> Timer & { return *this; }
  auto operator=(Timer && /*unused*/) noexcept -> Timer & { return *this; }
  Timer(Timer &&t) noexcept { *this = t; }
  Timer(const Timer &t) { *this = t; }
  auto GetName() { return TimerName; }
  auto GetElapsed() -> nanoseconds {
    if (IsRunning()) {
      return (NowNanoseconds() - TimeStarted) - TimeOffset;
    }
    return nanoseconds::zero();
  }
  static auto GetNow() -> nanoseconds { return NowNanoseconds(); }
  auto GetNowOffset() -> nanoseconds { return (NowNanoseconds() - TimeOffset); }
  auto GetRemaining() -> nanoseconds { return TimeLimit - GetElapsed(); }
  auto GetStart() -> nanoseconds { return TimeStarted; }
  auto GetResumed() -> nanoseconds { return TimeResumed; }
  auto GetPaused() -> nanoseconds { return TimePaused; }
  auto GetEnd() -> nanoseconds { return TimeStopped; }
  auto GetOffset() -> nanoseconds { return TimeOffset; }
  auto GetLimit() -> nanoseconds { return TimeLimit; }
  auto SetLimit(const nanoseconds &limit) -> void { TimeLimit = limit; }
  auto SetName(const std::string &name) -> void { TimerName = name; }
  static auto GetSeconds(nanoseconds nano) -> seconds {
    return seconds(nano.count() / 1000000000);
  }
  static auto GetMilliseconds(nanoseconds nano) -> milliseconds {
    return milliseconds(nano.count() / 1000000);
  }
  static auto GetMicroseconds(nanoseconds nano) -> microseconds {
    return microseconds(nano.count() / 1000);
  }
  static auto NowNanoseconds() -> nanoseconds {
    return high_resolution_clock::now().time_since_epoch();
  }
  static auto NowMicroseconds() -> microseconds {
    return microseconds(
        high_resolution_clock::now().time_since_epoch().count() / 1000);
  }
  static auto NowMilliseconds() -> milliseconds {
    return milliseconds(
        high_resolution_clock::now().time_since_epoch().count() / 1000000);
  }
  static auto NowSeconds() -> seconds {
    return seconds(high_resolution_clock::now().time_since_epoch().count() /
                   1000000000);
  }
  static auto NowMinutes() -> minutes {
    return minutes(high_resolution_clock::now().time_since_epoch().count() /
                   60000000000);
  }
  static auto NowHours() -> hours {
    return hours(high_resolution_clock::now().time_since_epoch().count() /
                 3600000000000);
  }
  static auto NowTimeT() -> time_t {
    return static_cast<time_t>(
        high_resolution_clock::now().time_since_epoch().count());
  }
  bool Is(int state) {
    if (state >= 0 && state <= 5) {
      return State == state;
    }
    return false;
  }
  auto Pause() -> bool {
    if (Is(Started) || Is(Resumed)) {
      TimePaused = GetNowOffset();
      State = Paused;
      return true;
    }
    return false;
  }
  auto Resume() -> bool {
    if (Is(Paused)) {
      TimeResumed = GetNowOffset();
      TimeOffset += TimeResumed - TimePaused;
      State = Resumed;
      return true;
    }
    return false;
  }
  auto Stop() -> bool {
    if (!Is(Stopped) && !Is(None)) {
      TimeStopped = GetNowOffset();
      State = Stopped;
      return true;
    }
    return false;
  }
  auto Start() -> bool {
    if (Is(None) || Is(Stopped) || Is(Restarted)) {
      TimeStarted = GetNowOffset();
      State = Started;
      return true;
    }
    return false;
  }
  auto Restart() -> bool {
    if (!Is(None)) {
      TimeResumed = nanoseconds::zero();
      TimePaused = nanoseconds::zero();
      TimeStarted = nanoseconds::zero();
      TimeStopped = nanoseconds::zero();
      TimeOffset = nanoseconds::zero();
      TimeRestarted = NowNanoseconds();
      State = Restarted;
      return true;
    }
    return false;
  }
  auto IsRunning() -> bool { return Is(Started) || Is(Resumed) || Is(Paused); }
};
} // namespace origin
#endif // TIMER_HPP