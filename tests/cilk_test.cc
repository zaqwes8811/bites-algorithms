
#include <gtest/gtest.h>

#include <cilk/cilk.h>
#include <assert.h>

int fib(int n) {
  if (n < 2)
    return n;
  int a = cilk_spawn fib(n-1);
  int b = fib(n-2);
  cilk_sync;
  return a + b;
}


TEST(Cilk, Base) {
  int result = fib(30);
  assert(result == 832040);
}
