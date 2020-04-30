#include <cstdint>
#include <vector>

using namespace std;

struct Permutation {
  vector<int64_t> container;
  Permutation(size_t size) : container(size) {
    for (int i = 0; i < size; i++) {
      container[i] = 0;
    }
  }
  size_t size() const { return container.size(); }
  int64_t& operator[](size_t i) { return container[i]; }
  const int64_t& operator[](size_t i) const { return container[i]; }
  Permutation operator*(const Permutation& other) const {
    int64_t n = container.size();
    Permutation result(n);
    for (int64_t i = 0; i < n; i++) {
      result[i] = (*this)[i] + other[(i + (*this)[i]) % n];
    }
    return result;
  }
};