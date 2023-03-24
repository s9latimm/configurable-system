#include <time.h>

void sleep(int) __attribute__((xray_never_instrument));
void sleep(long milliseconds) {
  struct timespec Rem;
  struct timespec Req = {(int)(milliseconds / 1000),
                         (milliseconds % 1000) * 1000000};
  nanosleep(&Req, &Rem);
}

#define FEATURE(name, iter)                                                    \
  void f_##name() __attribute__((xray_always_instrument));                     \
  void f_##name() { sleep(1); }
#include "Features.def"

int main(int argc, char *argv[]) {

#define FEATURE(name, iter)                                                    \
  bool F_##name __attribute__((feature_variable(#name))) = argc > 0;           \
  for (unsigned Iter = 0; Iter < iter; ++Iter) {                               \
    if (F_##name) {                                                            \
      f_##name();                                                              \
    }                                                                          \
  }
#include "Features.def"

  return 0;
}
