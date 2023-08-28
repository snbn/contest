#include <algorithm>
#include <cassert>
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
  const T& operator[](size_t index) const {
    const size_t N = container.size() / 2;
    assert(index < N);
    return container[index + N];
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
