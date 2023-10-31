#include <cstdint>
#include <vector>

/**
 * NOTE:
 * このセグメント木は不完全な実装のため、限定的なアクションにしか適用できない
 * 具体的には、f(m1)*f(m2) = f(m1*m2)を満たさなければならない
 */
template <typename Monoid, typename Action>
class LazySegmentTree {
  std::vector<Monoid> monoidContainer;
  std::vector<Action> actionContainer;

 public:
  LazySegmentTree(std::size_t size)
      : monoidContainer(msb1(size * 2 - 1), Monoid::identity()),
        actionContainer(msb1(size * 2 - 1), Action::empty()) {}

  void add(std::size_t start, std::size_t end, Action action) {
    add_inner(start, end, 0, monoidContainer.size() / 2, 1, action);
  }

  Monoid find(std::size_t start, std::size_t end) {
    if (start >= end) {
      return Monoid::identity();
    }
    return find_inner(start, end, 0, monoidContainer.size() / 2, 1);
  }

 private:
  void add_inner(std::size_t start, std::size_t end, std::size_t lb,
                 std::size_t ub, std::size_t node, Action action) {
    if (start == lb && end == ub) {
      actionContainer[node] = actionContainer[node] * action;
      maintain(node);
      return;
    }

    using std::size_t, std::min, std::max;
    const size_t mid = (lb + ub) / 2;
    const size_t left_child = node * 2;
    const size_t right_child = node * 2 + 1;
    if (mid > start) {
      add_inner(start, min(mid, end), lb, mid, left_child, action);
    }
    if (mid < end) {
      add_inner(max(mid, start), end, mid, ub, right_child, action);
    }
    maintain(left_child);
    maintain(right_child);
    monoidContainer[node] =
        monoidContainer[left_child] * monoidContainer[right_child];
  }

  Monoid find_inner(std::size_t start, std::size_t end, std::size_t lb,
                    std::size_t ub, std::size_t node) {
    maintain(node);
    if (start == lb && end == ub) {
      return monoidContainer[node];
    }

    using std::int64_t, std::size_t;
    using std::min, std::max;
    Monoid result = Monoid::identity();
    const size_t mid = (lb + ub) / 2;
    const size_t left_child = node * 2;
    const size_t right_child = node * 2 + 1;
    if (mid > start) {
      result = result * find_inner(start, min(mid, end), lb, mid, left_child);
    }
    if (mid < end) {
      result = result * find_inner(max(start, mid), end, mid, ub, right_child);
    }

    return result;
  }

  void maintain(std::size_t node) {
    monoidContainer[node] = actionContainer[node](monoidContainer[node]);
    size_t child = node * 2;
    if (child < actionContainer.size()) {
      actionContainer[child] = actionContainer[child] * actionContainer[node];
      actionContainer[child + 1] =
          actionContainer[child + 1] * actionContainer[node];
    }
    actionContainer[node] = Action::empty();
  }

  std::size_t msb1(std::size_t x) {
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;

    return x + 1;
  }
};