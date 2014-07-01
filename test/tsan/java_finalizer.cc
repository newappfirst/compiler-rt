// RUN: %clangxx_tsan -O1 %s -o %t && %run %t 2>&1 | FileCheck %s
#include "java.h"

void *Thread(void *p) {
  sleep(1);
  __tsan_java_finalize();
  *(int*)p = 42;
  return 0;
}

int main() {
  int const kHeapSize = 1024 * 1024;
  void *jheap = (char*)malloc(kHeapSize + 8) + 8;
  __tsan_java_init((jptr)jheap, kHeapSize);
  const int kBlockSize = 16;
  __tsan_java_alloc((jptr)jheap, kBlockSize);
  pthread_t th;
  pthread_create(&th, 0, Thread, jheap);
  *(int*)jheap = 43;
  pthread_join(th, 0);
  __tsan_java_free((jptr)jheap, kBlockSize);
  fprintf(stderr, "DONE\n");
  return __tsan_java_fini();
}

// CHECK-NOT: WARNING: ThreadSanitizer: data race
// CHECK: DONE