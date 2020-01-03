#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

template <typename T>
class SegmentTree {
  const T unit;
  const function<T(const T&, const T&)> op;
  vector<T> container;

 public:
  template <typename Op>
  SegmentTree(int size, const T& identity, const Op& operation)
      : unit(identity), op(operation), container(msb(size * 2 - 1)) {
    fill(container.begin(), container.end(), unit);
  }
  void update(size_t index, T value) {
    int n = container.size() / 2;
    container[index + n] = value;

    int node = (index + n) >> 1;
    while (node > 0) {
      container[node] = op(container[node << 1], container[(node << 1) + 1]);
      node >>= 1;
    }
  }
  T find(size_t s, size_t t) {
    if (s >= t) {
      return unit;
    }
    return find_inner(s, t, 0, container.size() / 2);
  }

 private:
  T find_inner(size_t s, size_t t, size_t lb, size_t ub) {
    if (s == lb && t == ub) {
      int n = container.size() / 2;
      return container[lca(n + s, n + t - 1)];
    }

    T result = unit;
    size_t mid = (lb + ub) / 2;
    if (mid > s) {
      result = op(result, find_inner(s, min(mid, t), lb, mid));
    }
    if (mid < t) {
      result = op(result, find_inner(max(s, mid), t, mid, ub));
    }

    return result;
  }
  int lca(int n0, int n1) {
    int a = n0 ^ n1;
    return n0 / msb(a);
  }
  int msb(int x) {
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;

    return x + 1;
  }
};

class BinaryIndexedTree {
  vector<int64_t> container;

 public:
  BinaryIndexedTree(size_t size) : container(size + 1) {
    fill(container.begin(), container.end(), 0);
  }
  void add(size_t i, int64_t v) {
    size_t idx = i + 1;
    while (idx < container.size()) {
      container[idx] += v;
      idx += idx & -idx;
    }
  }
  int64_t find(size_t i) {
    size_t idx = i;

    int64_t result = 0;
    while (idx != 0) {
      result = container[idx] + result;
      idx -= idx & -idx;
    }
    return result;
  }
  size_t lower_bound(int64_t v) {
    if (v == 0) {
      return 0;
    }
    int64_t w = v;
    size_t x = 0;
    for (size_t k = msb(container.size() - 1); k > 0; k >>= 1) {
      if (x + k < container.size() && container[x + k] < w) {
        w -= container[x + k];
        x += k;
      }
    }
    return x + 1;
  }

 private:
  size_t msb(size_t x) {
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return (x + 1) >> 1;
  }
};

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
  void add_inner(size_t s,
                 size_t t,
                 size_t lb,
                 size_t ub,
                 size_t node,
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