#include <cmath>
#include <vector>

class Vec;
Vec operator/(const Vec& v, const double& divisor);
Vec operator*(const double& scale, const Vec& v);

class Circle;
class Line;
template <typename G, typename H>
std::vector<Vec> intersection(const G& lh, const H& rh);
template <>
std::vector<Vec> intersection<Circle, Line>(const Circle& circle,
                                            const Line& line);
template <>
std::vector<Vec> intersection<Circle, Line>(const Circle& circle,
                                            const Line& line);

using namespace std;

class Vec {
  using F = double;
  vector<F> container;

 public:
  Vec(size_t size) : container(size) {}
  Vec(initializer_list<F> elements)
      : container(elements.begin(), elements.end()) {}
  size_t dim() const { return container.size(); }
  F& operator[](const size_t size) { return container.at(size); }
  const F& operator[](const size_t size) const { return container.at(size); }
  Vec operator+(const Vec& other) const {
    Vec ret(*this);
    for (size_t i = 0; i < dim(); i++) ret[i] += other[i];
    return ret;
  }
  Vec operator-(const Vec& other) const { return (*this) + (-other); }
  Vec operator-() const { return -1.0 * (*this); }
  F inner(const Vec& other) const {
    F acc = 0;
    for (size_t i = 0; i < dim(); i++) {
      acc += (*this)[i] * other[i];
    }
    return acc;
  }
  F norm() const { return sqrt(inner(*this)); }
  Vec unit() const { return (*this) / this->norm(); }
};
Vec operator/(const Vec& v, const double& divisor) { return 1.0 / divisor * v; }
Vec operator*(const double& scale, const Vec& v) {
  Vec ret(v);
  for (size_t i = 0; i < ret.dim(); i++) ret[i] *= scale;
  return ret;
}

class Circle {
  Vec m_center;
  double m_radius;

 public:
  Circle(const Vec& center, double radius)
      : m_center(center), m_radius(radius) {}
  const Vec& center() const { return m_center; }
  double radius() const { return m_radius; }
};

class Line {
  Vec m_grad;
  double m_bias;

 public:
  Line(const Vec& grad, double bias) : m_grad(grad), m_bias(bias) {}
  const Vec& grad() const { return m_grad; }
  double bias() const { return m_bias; }
};

template <>
vector<Vec> intersection<Circle, Circle>(const Circle& c0, const Circle& c1) {
  const Vec& p0 = c0.center();
  const Vec& p1 = c1.center();
  double r0 = c0.radius();
  double r1 = c1.radius();

  Vec grad({2.0 * (p1[0] - p0[0]), 2.0 * (p1[1] - p0[1])});
  const double bias = (r0 - r1) * (r0 + r1) -
                      (p0[0] - p1[0]) * (p0[0] + p1[0]) -
                      (p0[1] - p1[1]) * (p0[1] + p1[1]);
  return move(intersection(c0, Line(grad, bias)));
}
template <>
vector<Vec> intersection<Circle, Line>(const Circle& circle, const Line& line) {
  vector<Vec> result;

  const double g = line.grad().norm();
  const double dist = (line.bias() - line.grad().inner(circle.center())) / g;

  const double det = pow(circle.radius(), 2) - pow(dist, 2);
  if (det >= 0) {
    const double s = sqrt(det);
    const Vec u = line.grad().unit();
    const Vec p = circle.center() + dist * u;
    Vec dir({-u[1], u[0]});

    result.push_back(p + s * dir);
    result.push_back(p - s * dir);
  }

  return move(result);
}