#ifndef UTIL_HPP
#define UTIL_HPP
#include <chrono>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
namespace origin {
using namespace std::chrono;
using std::ios;
auto OpenFile(std::string filepath) {
  std::fstream fstream = std::fstream(filepath.c_str(), ios::in | ios::out);
  if (fstream.is_open()) {
    fstream.close();
  } else {
    fstream.open(filepath.c_str(), ios::in | ios::out);
  }
  return fstream;
}
inline auto CloseFile(std::fstream &fstream) {
  if (fstream.is_open()) {
    fstream.close();
  } else {
    return -1;
  }
  return 0;
}
auto ToString(int i) { return std::to_string(i); }
auto ToString(const char *s) -> std::string { return {s}; }
auto ToString(nanoseconds nano) {
  return std::to_string(nano.count() / 1000000000.0);
}
inline auto ToString(microseconds mic) {
  return std::to_string(mic.count() / 1000000.0);
}
inline auto ToString(milliseconds mil) {
  return std::to_string(mil.count() / 1000.0);
}
inline auto ToString(seconds sec) { return std::to_string(sec.count()); }
inline auto ToLongLong(time_point<nanoseconds> start,
                       time_point<nanoseconds> end) -> long long {
  if (start.time_since_epoch() < end.time_since_epoch()) {
    return start.time_since_epoch().count() - end.time_since_epoch().count();
  }
  return end.time_since_epoch().count() - start.time_since_epoch().count();
}
inline auto ToLongLong(time_point<microseconds> start,
                       time_point<microseconds> end) -> long long {
  if (start.time_since_epoch() < end.time_since_epoch()) {
    return start.time_since_epoch().count() - end.time_since_epoch().count();
  }
  return end.time_since_epoch().count() - start.time_since_epoch().count();
}
inline auto ToLongLong(time_point<milliseconds> start,
                       time_point<milliseconds> end) -> long long {
  if (start.time_since_epoch() < end.time_since_epoch()) {
    return start.time_since_epoch().count() - end.time_since_epoch().count();
  }
  return end.time_since_epoch().count() - start.time_since_epoch().count();
}
inline auto ToLongLong(time_point<seconds> start, time_point<seconds> end)
    -> long long {
  if (start.time_since_epoch() < end.time_since_epoch()) {
    return start.time_since_epoch().count() - end.time_since_epoch().count();
  }
  return end.time_since_epoch().count() - start.time_since_epoch().count();
}
inline auto ToInt(time_point<minutes> start, time_point<minutes> end) {
  if (start.time_since_epoch() < end.time_since_epoch()) {
    return static_cast<int>(start.time_since_epoch().count() -
                            end.time_since_epoch().count());
  }
  return static_cast<int>(end.time_since_epoch().count() -
                          start.time_since_epoch().count());
}
inline auto ToInt(time_point<hours> start, time_point<hours> end) {
  if (start.time_since_epoch() < end.time_since_epoch()) {
    return static_cast<int>(start.time_since_epoch().count() -
                            end.time_since_epoch().count());
  }
  return static_cast<int>(end.time_since_epoch().count() -
                          start.time_since_epoch().count());
}
inline auto ToNanoseconds(time_point<high_resolution_clock> tp) -> nanoseconds {
  return duration_cast<nanoseconds>(tp.time_since_epoch());
}
inline auto ToMicroseconds(time_point<high_resolution_clock> tp)
    -> microseconds {
  return duration_cast<microseconds>(tp.time_since_epoch());
}
inline auto ToMilliseconds(time_point<high_resolution_clock> tp)
    -> milliseconds {
  return duration_cast<milliseconds>(tp.time_since_epoch());
}
inline auto ToSeconds(time_point<high_resolution_clock> tp) -> seconds {
  return duration_cast<seconds>(tp.time_since_epoch());
}
inline auto ToMinutes(time_point<high_resolution_clock> tp) -> minutes {
  return duration_cast<minutes>(tp.time_since_epoch());
}
inline auto ToHours(time_point<high_resolution_clock> tp) -> hours {
  return duration_cast<hours>(tp.time_since_epoch());
}
inline auto ToTimeT(time_point<high_resolution_clock> tp) -> time_t {
  return duration_cast<seconds>(tp.time_since_epoch()).count();
}
inline auto ToClockT(time_point<high_resolution_clock> tp) -> clock_t {
  return duration_cast<nanoseconds>(tp.time_since_epoch()).count();
}
inline auto ToClockT(nanoseconds ns) -> clock_t {
  return nanoseconds(ns).count();
}
} // namespace origin
#endif // UTIL_HPP
