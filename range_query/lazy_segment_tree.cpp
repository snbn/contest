#include <cstdint>
#include <vector>

using namespace std;

class LazySegmentTree {
  vector<int64_t> container;
  vector<int64_t> lazyContainer;

 public:
  LazySegmentTree(int size)
      : container(msb1(size * 2 - 1)), lazyContainer(msb1(size * 2 - 1)) {}

  void add(size_t s, size_t t, int64_t x) {
    add_inner(s, t, 0, container.size() / 2, 1, x);
  }

  int64_t find(size_t s, size_t t) {
    if (s >= t) {
      return 0;
    }
    return find_inner(s, t, 0, container.size() / 2, 1);
  }

 private:
  void add_inner(size_t s, size_t t, size_t lb, size_t ub, size_t node,
                 int64_t x) {
    if (s == lb && t == ub) {
      lazyContainer[node] += x;
      maintain(node);
      return;
    }

    size_t mid = (lb + ub) / 2;
    if (mid > s) {
      add_inner(s, min(mid, t), lb, mid, node * 2, x);
    }
    if (mid < t) {
      add_inner(max(mid, s), t, mid, ub, node * 2 + 1, x);
    }
    maintain(node * 2);
    maintain(node * 2 + 1);
    container[node] = max(container[node * 2], container[node * 2 + 1]);
  }

  int64_t find_inner(size_t s, size_t t, size_t lb, size_t ub, size_t node) {
    maintain(node);
    if (s == lb && t == ub) {
      return container[node];
    }

    int64_t result = 0;
    size_t mid = (lb + ub) / 2;
    if (mid > s) {
      result = max(result, find_inner(s, min(mid, t), lb, mid, node * 2));
    }
    if (mid < t) {
      result = max(result, find_inner(max(s, mid), t, mid, ub, node * 2 + 1));
    }

    return result;
  }
  void maintain(size_t node) {
    container[node] += lazyContainer[node];
    size_t child = node * 2;
    if (child < container.size()) {
      lazyContainer[child] += lazyContainer[node];
      lazyContainer[child + 1] += lazyContainer[node];
    }
    lazyContainer[node] = 0;
  }
  int msb1(int x) {
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;

    return x + 1;
  }
};