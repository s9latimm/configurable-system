#include <algorithm>
#include <iostream>
#include <time.h>
#include <vector>

struct Workload {
  static void sleep(unsigned Milliseconds)
      __attribute__((xray_never_instrument)) {
    struct timespec Time = {Milliseconds / 1'000,
                            (Milliseconds % 1'000) * 1'000'000};
    nanosleep(&Time, nullptr);
  }

  static void busy() __attribute__((xray_never_instrument)) {
    for (unsigned Iter = 0; Iter < 10'000; ++Iter) {
      asm volatile("" : "+g"(Iter) : :);
    }
  }

  static void collatz() __attribute__((xray_never_instrument)) {
    for (unsigned Number = std::rand(); Number > 1;
         Number = Number % 2 ? 3 * Number + 1 : Number >> 1) {
      asm volatile("nop");
    }
  }

  static void sort() __attribute__((xray_never_instrument)) {
    std::vector<unsigned> Values;
    while (Values.size() < 42) {
      Values.push_back(std::rand());
      for (unsigned Iter = 1; Iter < Values.size(); ++Iter) {
        unsigned Value = Values[Iter];
        unsigned Index = Iter;
        for (; Index > 0 && Values[Index - 1] > Value; --Index) {
          Values[Index] = Values[Index - 1];
        }
        Values[Index] = Value;
      }
      std::vector<unsigned> Copy;
      for (unsigned Index = Values.size(); Index > 0; --Index) {
        Copy.push_back(Values[Index - 1]);
      }
      Values = Copy;
      asm volatile("nop");
    }
  }

  static void run(unsigned Milliseconds, void (*Workload)())
      __attribute__((xray_never_instrument)) {
    struct timespec Time;
    timespec_get(&Time, TIME_UTC);
    unsigned Start = Time.tv_sec * 1'000 + Time.tv_nsec / 1'000'000;
    for (unsigned End = Start + Milliseconds; Start < End;
         Start = Time.tv_sec * 1'000 + Time.tv_nsec / 1'000'000) {
      Workload();
      timespec_get(&Time, TIME_UTC);
    }
  }

  static void run(unsigned Milliseconds, void (*Workload)(unsigned))
      __attribute__((xray_never_instrument)) {
    Workload(Milliseconds);
  }
};

#define FEATURE(name, iter, workload)                                          \
  void f_##name() __attribute__((xray_always_instrument)) {                    \
    Workload::run(WORKLOAD, &Workload::workload);                              \
  }
#include "Features.def"

int main(int argc, char *argv[]) {

  std::srand(42);

#define FEATURE(name, iter, workload)                                          \
  bool F_##name __attribute__((feature_variable(#name))) = argc > 0;           \
  std::cout << "FEATURE(" << #name << ", " << #iter << ", " << #workload       \
            << ")" << std::endl;                                               \
  for (unsigned Iter = 0; Iter < iter; ++Iter) {                               \
    Workload::run(0, &Workload::workload);                                     \
    if (F_##name) {                                                            \
      Workload::run(0, &Workload::workload);                                   \
      f_##name();                                                              \
      Workload::run(0, &Workload::workload);                                   \
    }                                                                          \
    Workload::run(0, &Workload::workload);                                     \
  }
#include "Features.def"

  return 0;
}
