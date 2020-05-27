#include <cstdint>

using std::int64_t;

template <int64_t MOD>
class Modint {
  using Self = Modint<MOD>;

  int64_t m_value;

 public:
  explicit Modint(int64_t value) : m_value((value % MOD + MOD) % MOD) {}
  Self& operator=(const Self& rh) {
    m_value = rh.m_value;
    return *this;
  }
  Self operator-() const { return Self(-m_value); }
  Self operator+(const Self& other) const {
    return Self(m_value + other.m_value);
  }
  Self operator-(const Self& other) const {
    return Self(m_value - other.m_value);
  }
  Self operator*(const Self& other) const {
    return Self(m_value * other.m_value);
  }
  Self& operator+=(const Self& rh) { return (*this) = (*this) + rh; }
  Self& operator-=(const Self& rh) { return (*this) = (*this) - rh; }
  Self& operator*=(const Self& rh) { return (*this) = (*this) * rh; }
  int64_t value() const { return m_value; }
};