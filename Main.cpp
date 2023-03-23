#include <chrono>
#include <iostream>
#include <thread>

#define FEATURE(name)                                                          \
  void f_##name() {                                                            \
    std::cout << "ENTER " << #name << std::endl;                               \
    std::this_thread::sleep_for(std::chrono::milliseconds(100));               \
    std::cout << "EXIT " << #name << std::endl;                                \
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
