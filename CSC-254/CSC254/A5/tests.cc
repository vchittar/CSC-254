#include "sets.h"
#include <assert.h>


/**
 * The following is provided ONLY to make sure your implementations compiles
 * The code added by the TAs is not intended to be examples of good c++ code.
 *
 * Assert statemets terminate the program if the experesion is not true (non-zero).
 * All asserts written here should pass. When actually testing code for correctness,
 * we will not be using asserts (so early failing tests won't cause later tests to not run)
 *
 */

void carray_simple_set_tests() {
    carray_simple_set<int> i(0, 100);
    for (int x = 0; x < 100; x+=3)
      i += x;
    i -= 6;
    i += 7;
    assert(i.contains(3));
    assert(i.contains(7));
    assert(!i.contains(6));
}

void carray_range_set_tests() {
    carray_range_set<int> s(0, 100);
    s += range<int>(4, true, 9, false);
    s += range<int>(11, true, 15, true);
    s -= range<int>(15, true, 15, true);
    assert(!s.contains(15));
    assert(s.contains(4));
    assert(!s.contains(98));
    range_set<char> * d = new carray_range_set<char>('a', 'z');
    *d += range<char>('a', true, 'd', false);
    *d += range<char>('g', true, 'i', false);
    *d -= range<char>('b', true, 'c', false);
    assert(!d->contains('y'));
    assert(d->contains('a'));
    assert(!d->contains('d'));
}

void hash_simple_tests() {
    hashed_simple_set<int> s(100);
    s += 4; s+= 15; s+= 3;
    assert(s.contains(15));
    assert(s.contains(4));
    assert(!s.contains(98));
    hashed_simple_set<char> d(30);
    d += 'd';
    d += 'z';
    d -= 'd';
    assert(d.contains('z'));
    assert(!d.contains('d'));
    assert(!d.contains('a'));
}

void hash_range_tests() {
    hashed_range_set<int> s(100);
    s += range<int>(4, true, 9, false);
    s += range<int>(11, true, 15, true);
    s -= range<int>(15, true, 15, true);
    assert(!s.contains(15));
    assert(s.contains(4));
    assert(!s.contains(98));
    range_set<char> * d = new hashed_range_set<char>(30);
    *d += range<char>('a', true, 'd', false);
    *d += range<char>('g', true, 'i', false);
    *d -= range<char>('b', true, 'c', false);
    assert(!d->contains('z'));
    assert(d->contains('a'));
    assert(!d->contains('d'));
}

class double_cmp {
  public:
  bool precedes(double a, double b) const {
    return a < b;
  }
  bool equals(double a, double b) const {
    return a == b;
  }
};

void bin_search_simple_set_tests() {
  bin_search_simple_set<double, double_cmp> s(10);
  s += 3.7;
  s += 8.6;
  assert(s.contains(3.7));
  assert(!s.contains(4.2));
  s -= 3.7;
  assert(!s.contains(3.7));
}

void bin_search_range_set_tests() {
  bin_search_range_set<double, double_cmp> s(10);
  s += range<double, double_cmp>(2.0, true, 4.0, true);
  s += range<double, double_cmp>(6.0, true, 8.0, true);
  s += range<double, double_cmp>(10.0, true, 11.0, true);
  assert(!s.contains(5.0));
  assert(s.contains(3.7));
  s += range<double, double_cmp>(1.0, false, 10.7, false);
  assert(s.contains(5.0));
}

int main() {
  carray_simple_set_tests();
  carray_range_set_tests();
  hash_simple_tests();
  hash_range_tests();
  bin_search_simple_set_tests();
  bin_search_range_set_tests();
}
