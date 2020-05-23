#include <algorithm>
#include <cmath>
#include <vector>

class Vec;
Vec operator/(const Vec& v, const double& divisor);
Vec operator*(const double& scale, const Vec& v);

class Circle;
class Line;
template <typename G, typename H>
std::pair<std::vector<Vec>, bool> intersection(const G& lh, const H& rh,
                                               double eps);
template <>
std::pair<std::vector<Vec>, bool> intersection<Circle, Circle>(
    const Circle& circle, const Circle& line, double eps);
template <>
std::pair<std::vector<Vec>, bool> intersection<Circle, Line>(
    const Circle& circle, const Line& line, double eps);
template <>
std::pair<std::vector<Vec>, bool> intersection(const Line& lh, const Line& rh,
                                               double eps);

using namespace std;

class Vec {
  using F = double;
  vector<F> container;

 public:
  explicit Vec(size_t size) : container(size) {}
  explicit Vec(initializer_list<F> elements)
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
  Vec& operator+=(const Vec& other) {
    for (size_t i = 0; i < dim(); i++) {
      (*this)[i] += other[i];
    }
    return *this;
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

  static const int ONLINE = 0x7;
  static const int ONSEGMENT = 0x1;
  static const int FRONT = 0x2;
  static const int BACK = 0x4;
  static const int OFFLINE = 0x18;
  static const int CCW = 0x8;
  static const int CW = 0x10;
  int check_rel(const Vec& p, double eps) const {
    const Vec u = direction();
    const Vec v = p - m_start;
    const double uLen = u.norm();

    const double a = u.ccw(v);

    int result = 0;
    if (a > eps) {
      result |= CCW;
    } else if (a < -eps) {
      result |= CW;
    } else {
      const double det = u.inner(v) / uLen;
      if (det < -eps) {
        result |= BACK;
      } else if (det > uLen + eps) {
        result |= FRONT;
      } else {
        result |= ONSEGMENT;
      }
    }
    return result;
  }
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

class Polygon {
  const vector<Vec>& vert;

 public:
  Polygon(const vector<Vec>& vert) : vert(vert) {}
  double area() const {
    const size_t n = vert.size();
    double acc = 0;
    for (size_t i = 0; i < n; i++) {
      acc += vert[i].ccw(vert[(i + 1) % n]);
    }
    return acc / 2.0;
  }
  bool isConvex(double eps) const {
    const size_t n = vert.size();
    for (size_t i = 0; i < n; i++) {
      const Vec a = vert[(i + 1) % n] - vert[i],
                b = vert[(i + 2) % n] - vert[i];
      const double c = a.ccw(b);
      if (c < -eps) {
        return false;
      }
    }
    return true;
  }

  static const int ON_EDGE = 0x3;
  static const int CONTAINED = 0x1;
  static const int NOT_CONTAINED = 0x2;
  int check_rel(const Vec& p, double eps) const {
    const size_t n = vert.size();
    double angle = 0;
    for (size_t i = 0; i < n; i++) {
      const Vec &s = vert[i], &t = vert[(i + 1) % n];

      if (Segment(s, t).check_rel(p, eps) == Segment::ONSEGMENT) {
        return ON_EDGE;
      }

      const Vec u = s - p, v = t - p;
      const double theta = atan2(u.ccw(v), u.inner(v));
      angle += theta;
    }
    if (angle < 3) {  // check if 0 or 2*PI
      return NOT_CONTAINED;
    } else {
      return CONTAINED;
    }
  }
};

class Convex {
  const vector<Vec>& m_points;
  vector<int> m_hull;

 public:
  Convex(const vector<Vec>& points) : m_points(points), m_hull() {
    const size_t n = m_points.size();

    vector<int> idx(n);
    for (size_t i = 0; i < n; i++) idx[i] = i;
    sort(idx.begin(), idx.end(), [&](const int& lh, const int& rh) {
      if (m_points[lh][0] == m_points[rh][0]) {
        return m_points[lh][1] < m_points[rh][1];
      }
      return m_points[lh][0] < m_points[rh][0];
    });

    vector<int> conv[2];
    double si = 1;
    for (int j = 0; j < 2; j++) {
      vector<int>& cv = conv[j];
      for (size_t i = 0; i < n; i++) {
        const int k = idx[i];
        const Vec& c = m_points[k];
        while (cv.size() >= 2 &&
               si * (m_points[cv[cv.size() - 1]] - m_points[cv[cv.size() - 2]])
                           .ccw(c - m_points[cv[cv.size() - 2]]) <
                   0) {
          cv.pop_back();
        }
        cv.push_back(k);
      }
      si *= -1;
    }

    copy(conv[0].begin(), conv[0].end(), back_inserter(m_hull));
    copy(next(conv[1].rbegin()), prev(conv[1].rend()), back_inserter(m_hull));
  }
  double diameter() const {
    double result = 0;
    const size_t n = m_hull.size();
    int j = 0;
    for (size_t i = 0; i < n; i++) {
      double d = ((*this)[i] - (*this)[j % n]).norm();
      double e = ((*this)[i] - (*this)[(j + 1) % n]).norm();
      while (d < e) {
        j++;
        d = e;
        e = ((*this)[i] - (*this)[(j + 1) % n]).norm();
      }
      result = max(result, d);
    }
    return result;
  }
  size_t size() const { return m_hull.size(); }
  const vector<int>& ids() const { return m_hull; }
  const Vec& operator[](size_t index) const {
    return m_points.at(m_hull[index % size()]);
  }
};

template <>
pair<vector<Vec>, bool> intersection<Circle, Circle>(const Circle& c0,
                                                     const Circle& c1,
                                                     double eps) {
  const Vec& p0 = c0.center();
  const Vec& p1 = c1.center();
  double r0 = c0.radius();
  double r1 = c1.radius();

  const Vec v = p0 - p1;

  if (r0 + r1 < v.norm()) {
    return make_pair(vector<Vec>(), false);
  } else if (r0 > r1 + v.norm() || r1 > r0 + v.norm()) {
    return make_pair(vector<Vec>(), false);
  } else {
    Vec grad = 2.0 * (p1 - p0);
    const double bias = (r0 - r1) * (r0 + r1) -
                        (p0[0] - p1[0]) * (p0[0] + p1[0]) -
                        (p0[1] - p1[1]) * (p0[1] + p1[1]);
    return move(intersection(c0, Line(grad, bias), eps));
  }
}
template <>
pair<vector<Vec>, bool> intersection<Circle, Line>(const Circle& circle,
                                                   const Line& line,
                                                   double eps) {
  const double g = line.grad().norm();

  if (g < eps) {
    return make_pair(vector<Vec>(), abs(line.bias()) < eps);
  } else {
    vector<Vec> result;
    const double dist = (line.bias() - line.grad().inner(circle.center())) / g;

    const double det = pow(circle.radius(), 2) - pow(dist, 2);
    if (det < 0) {
    } else {
      const double s = sqrt(det);
      const Vec u = line.grad().unit();
      const Vec p = circle.center() + dist * u;
      Vec dir = u.normal();

      result.push_back(p + s * dir);
      result.push_back(p - s * dir);
    }

    return make_pair(move(result), false);
  }
}

template <>
pair<vector<Vec>, bool> intersection(const Line& lh, const Line& rh,
                                     double eps) {
  const Vec &g1 = lh.grad(), g2 = rh.grad();
  const double c1 = lh.bias(), c2 = rh.bias();
  const Vec g = g1.unit();
  const Vec h = g.normal();

  const double det = h.inner(g2);

  if (abs(det) / g2.norm() < eps) {
    return make_pair(vector<Vec>(), abs(c1 * g2.norm() - c2 * g1.norm()) < eps);
  } else {
    const double k = (c2 - g.inner(g2) * c1 / g1.norm()) / det;
    vector<Vec> result = vector<Vec>(1, c1 / g1.norm() * g + k * h);
    return make_pair(result, false);
  }
}
