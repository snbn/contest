#include <cassert>
#include <cstdint>
#include <vector>

class MonotoneCHT {
  using ValType = std::int64_t;
  struct Line {
    ValType a, b;
  };
  std::vector<Line> lines;
  std::size_t last_index = 0;

 public:
  void add(ValType a, ValType b) {
    const Line l{.a = a, .b = b};
    while (lines.size() >= 2 &&
           check(lines[lines.size() - 2], lines[lines.size() - 1], l)) {
      lines.pop_back();
    }
    lines.push_back(l);
  }
  ValType get(ValType x) {
    if (last_index >= lines.size()) {
      last_index = lines.size() - 1;
    }
    while (is_decreasing(last_index, x)) {
      last_index++;
    }
    const auto& l = lines[last_index];
    return l.a * x + l.b;
  }

 private:
  bool check(const Line& l1, const Line& l2, const Line& new_line) {
    assert(l1.a > l2.a);
    assert(l2.a > new_line.a);
    // 交点の座標をxとしたときの、 a*x+b < a_1*x+b_1
    // 定数項が常に正である場合に最もオーバーフローしにくい計算順
    return (l2.a - new_line.a) * l1.b <
           (l1.a - new_line.a) * l2.b - (l1.a - l2.a) * new_line.b;
  }
  bool is_decreasing(std::size_t last_index, ValType x) {
    return (last_index < lines.size() - 1) &&
           lines[last_index].a * x + lines[last_index].b >=
               lines[last_index + 1].a * x + lines[last_index + 1].b;
  }
};