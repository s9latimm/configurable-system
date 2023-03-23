#include <chrono>
#include <iostream>
#include <thread>

void sleep(int) __attribute__((xray_never_instrument));
void sleep(int milliseconds) {
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

#define FEATURE(name)                                                          \
  void f_##name() __attribute__((xray_always_instrument));                     \
  void f_##name() {                                                            \
    std::cout << #name << std::endl;                                           \
    sleep(10);                                                                 \
  }
#include "Features.def"

int main(int argc, char *argv[]) {

#define FEATURE(name)                                                          \
  bool F_##name __attribute__((feature_variable(#name))) = argc > 0;           \
  if (F_##name) {                                                              \
    f_##name();                                                                \
  }
#include "Features.def"

  return 0;
}
