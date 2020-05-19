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
std::vector<Vec> intersection<Circle, Circle>(const Circle& circle,
                                              const Circle& line);
template <>
std::vector<Vec> intersection<Circle, Line>(const Circle& circle,
                                            const Line& line);
template <>
vector<Vec> intersection(const Line& lh, const Line& rh);

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
  F ccw(const Vec& other) const {
    return (*this)[0] * other[1] - other[0] * (*this)[1];
  }
  Vec normal() const { return Vec({-(*this)[1], (*this)[0]}); }
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
  static Line fromPoints(const Vec& p0, const Vec& p1) {
    Vec d = p0 - p1;
    Vec g({-d[1], d[0]});
    return Line(g, g.inner(p0));
  }
  const Vec& grad() const { return m_grad; }
  double bias() const { return m_bias; }
  double dist(const Vec& p) const {
    return abs(m_grad.inner(p) - m_bias) / m_grad.norm();
  }
  Vec projection(const Vec& p) const {
    const Vec g = m_grad.unit();
    const Vec h({-g[1], g[0]});
    return m_bias / m_grad.norm() * g + h.inner(p) * h;
  }
};

class Segment {
  const Vec m_start;
  const Vec m_end;

 public:
  Segment(const Vec& start, const Vec& end) : m_start(start), m_end(end) {}
  bool intersect(const Segment& other, const double eps) const {
    const Vec u = other.m_start - m_start, w = other.m_end - m_start;
    const Vec v0 = m_end - m_start, v1 = other.m_end - other.m_start;

    const double vn = v0.norm();
    double u_v = v0.inner(u) / vn;
    double w_v = v0.inner(w) / vn;
    if (abs(abs(u_v / u.norm()) - 1) < eps &&
        abs(abs(w_v / w.norm()) - 1) < eps) {
      if (u_v > w_v) {
        swap(u_v, w_v);
      }
      return !(u_v > vn + eps || w_v < -eps);
    }
    bool cond1 =
        v0.ccw(other.m_start - m_start) * v0.ccw(other.m_end - m_start) < eps;
    bool cond2 =
        v1.ccw(m_start - other.m_start) * v1.ccw(m_end - other.m_start) < eps;
    return cond1 && cond2;
  }
  Vec direction() const { return m_end - m_start; }
  double dist(const Vec& p) const {
    const Vec dir = direction();
    const Vec d = dir.unit();
    const Vec q = p - m_start;
    const double a = d.inner(q);
    if (a < 0) {
      return (m_start - p).norm();
    } else if (dir.norm() < a) {
      return (m_end - p).norm();
    } else {
      return abs(d.normal().inner(q));
    }
  }
  double dist(const Segment& other) const {
    if (intersect(other, 0.0)) {
      return 0.0;
    }
    return min(min(dist(other.m_start), dist(other.m_end)),
               min(other.dist(m_start), other.dist(m_end)));
  }
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
template <>
vector<Vec> intersection(const Line& lh, const Line& rh) {
  const Vec &g1 = lh.grad(), g2 = rh.grad();
  const double c1 = lh.bias(), c2 = rh.bias();
  const Vec g = g1.unit();
  const Vec h({-g[1], g[0]});

  const double k = (c2 - g.inner(g2) * c1 / g1.norm()) / h.inner(g2);
  vector<Vec> result = vector<Vec>(1, c1 / g1.norm() * g + k * h);
  return result;
}
