#ifndef TIMER_HPP
#define TIMER_HPP

#ifdef DEBUG
#define IS_DEBUG true
#else
#define IS_DEBUG false
#endif

#include <chrono>
#include <iostream>
#include <string>

class Timer {
private:
  std::string name;
  std::chrono::_V2::system_clock::time_point start;
  std::chrono::_V2::system_clock::time_point end;

  bool ignore_debug;

public:
  Timer(const std::string &name, const bool &ignore_debug = false)
      : name(name), ignore_debug(ignore_debug) {
    start = std::chrono::high_resolution_clock::now();
  }
  ~Timer() {
    end = std::chrono::high_resolution_clock::now();
    if (ignore_debug || IS_DEBUG) {
      display_time();
    }
  }

  void display_time() {
    std::cout << name << " : "
              << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                       start)
              << std::endl;
  }
};

#define TIMEIT(func)                                                           \
  [&]() {                                                                      \
    std::string func_name = std::format("{}, line : {}, file : {}\n\t", #func, \
                                        __LINE__, __FILE__);                   \
    std::replace(func_name.begin(), func_name.end(), '(', '_');                \
    std::replace(func_name.begin(), func_name.end(), ')', '_');                \
    Timer timer(func_name);                                                    \
    return func;                                                               \
  }()

#endif