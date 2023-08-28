#include <algorithm>
#include <cstdint>
#include <ostream>
#include <vector>

template <typename F>
class Matrix {
  size_t m_col;
  std::vector<F> m_container;

 public:
  explicit Matrix(size_t row, size_t col)
      : m_col(col), m_container(row * col, (F)0) {}
  static Matrix eye(size_t row, size_t col) {
    Matrix ret(row, col);
    for (size_t i = 0; i < row; i++) ret(i, i) = 1;
    return ret;
  }
  size_t row() const { return m_container.size() / m_col; }
  size_t col() const { return m_col; }
  const F& operator()(size_t i, size_t j) const {
    return m_container[i * col() + j];
  }
  F& operator()(size_t i, size_t j) { return m_container[i * col() + j]; }
  void swap_row(size_t i, size_t j) {
    swap_ranges(m_container.begin() + i * col(),
                m_container.begin() + (i + 1) * col(),
                m_container.begin() + j * col());
  }
  void scale_row(size_t i, F s) {
    transform(m_container.begin() + i * col(),
              m_container.begin() + (i + 1) * col(),
              m_container.begin() + i * col(),
              [&](const F& value) { return value * s; });
  }
  void add_row(size_t i, size_t j, F s) {
    transform(m_container.begin() + i * col(),
              m_container.begin() + (i + 1) * col(),
              m_container.begin() + j * col(), m_container.begin() + i * col(),
              [&](const F& v0, const F& v1) { return v0 + s * v1; });
  }
  void gauss_jordan_forward(Matrix& b) {
    Matrix &lh = *this, &rh = b;
    size_t rank = 0;
    for (size_t i = 0; i < lh.col(); i++) {
      for (size_t j = rank; j < lh.row(); j++) {
        if (!is_zero(lh(j, i))) {
          lh.swap_row(rank, j);
          rh.swap_row(rank, j);

          const F s = lh(rank, i);
          lh.scale_row(rank, (F)1 / s);
          rh.scale_row(rank, (F)1 / s);

          for (size_t j = rank + 1; j < lh.row(); j++) {
            const F s = lh(j, i);
            lh.add_row(j, rank, -s);
            rh.add_row(j, rank, -s);
          }
          rank++;
          break;
        }
      }
    }
  }
  void gauss_jordan_backward(Matrix& b) {
    Matrix &lh = *this, &rh = b;
    for (int i = lh.col() - 1; i >= 0; i--) {
      for (int j = 0; j < i; j++) {
        const F s = -lh(j, i);
        lh.add_row(j, i, s);
        rh.add_row(j, i, s);
      }
    }
  }
  size_t rank() const {
    size_t j = 0;
    for (size_t i = 0; i < row(); i++) {
      while (j < col() && is_zero((*this)(i, j))) j++;
      if (j == col()) return i;
    }
  }

 private:
  static bool is_zero(const F& v) { return v == (F)0; };
};

template <>
bool Matrix<double>::is_zero(const double& v) {
  const double EPS = 1e-9;
  return abs(v) < EPS;
}

template <typename F>
std::ostream& operator<<(std::ostream& out, const Matrix<F>& m) {
  out << "[\n";
  for (size_t i = 0; i < m.row(); i++) {
    out << " [";
    for (size_t j = 0; j < m.col(); j++)
      out << setw(16) << fixed << setprecision(9) << m(i, j) << " , ";
    out << "],\n";
  }
  out << "]\n";
  return out;
}