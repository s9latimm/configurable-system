#include <iostream>
#include <unistd.h>

void foo(int);
void bar(int);
void buz(int);

void foo(int depth) {
  if (depth > 0) {
    std::cout << "ENTER Foo" << std::endl;
    sleep(1);
    bar(depth - 1);
    std::cout << "EXIT Foo" << std::endl;
    return;
  }
}

void bar(int depth) {
  if (depth > 0) {
    std::cout << "ENTER Bar" << std::endl;
    sleep(2);
    buz(depth - 1);
    std::cout << "EXIT Bar" << std::endl;
    return;
  }
}

void buz(int depth) {
  if (depth > 0) {
    std::cout << "ENTER Buz" << std::endl;
    sleep(3);
    foo(depth - 1);
    std::cout << "EXIT Buz" << std::endl;
    return;
  }
}

int main(int argc, char *argv[]) {

  bool Foo __attribute__((feature_variable("Foo"))) = argc > 1;
  bool Bar __attribute__((feature_variable("Bar"))) = argc > 2;
  bool Buz __attribute__((feature_variable("Buz"))) = argc > 3;

  if (Foo) {
    foo(argc);
  }

  if (Bar) {
    bar(argc);
  }

  if (Buz) {
    buz(argc);
  }

  return 0;
}
