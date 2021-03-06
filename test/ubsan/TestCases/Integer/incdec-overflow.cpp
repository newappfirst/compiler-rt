// RUN: %clangxx -DOP=n++ -fsanitize=signed-integer-overflow %s -o %t && %run %t 2>&1 | FileCheck %s --check-prefix=PLUS
// RUN: %clangxx -DOP=++n -fsanitize=signed-integer-overflow %s -o %t && %run %t 2>&1 | FileCheck %s --check-prefix=PLUS
// RUN: %clangxx -DOP=m-- -fsanitize=signed-integer-overflow %s -o %t && %run %t 2>&1 | FileCheck %s --check-prefix=MINUS
// RUN: %clangxx -DOP=--m -fsanitize=signed-integer-overflow %s -o %t && %run %t 2>&1 | FileCheck %s --check-prefix=MINUS

#include <stdint.h>

int main() {
  int n = 0x7ffffffd;
  n++;
  n++;
  int m = -n - 1;
  OP;
  // PLUS: incdec-overflow.cpp:[[@LINE-1]]:3: runtime error: signed integer overflow: 2147483647 + 1 cannot be represented in type 'int'
  // MINUS: incdec-overflow.cpp:[[@LINE-2]]:3: runtime error: signed integer overflow: -2147483648 - 1 cannot be represented in type 'int'
}
