#include <iostream>
#include <unistd.h>

void foo() {
  std::cout << "ENTER Foo" << std::endl;
  sleep(3);
  std::cout << "EXIT Foo" << std::endl;
  return;
}

void bar() {
  std::cout << "ENTER Bar" << std::endl;
  sleep(4);
  std::cout << "EXIT Bar" << std::endl;
  return;
}

void buz() {
  std::cout << "ENTER Buz" << std::endl;
  sleep(5);
  std::cout << "EXIT Buz" << std::endl;
  return;
}

int main(int argc, char *argv[]) {

  bool Foo __attribute__((feature_variable("Foo"))) = argc > 1;
  bool Bar __attribute__((feature_variable("Bar"))) = argc > 2;
  bool Buz __attribute__((feature_variable("Buz"))) = argc > 3;

  if (Foo) {
    foo();
  }

  if (Bar) {
    bar();
  }

  if (Buz) {
    buz();
  }

  return 0;
}
