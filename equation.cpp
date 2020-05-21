#include <cmath>
#include <tuple>
#include <vector>

using std::pair;
using std::vector;

pair<vector<double>, bool> solve_linear(double a, double b, double eps);
pair<vector<double>, bool> solve_quad(double a, double b, double c, double eps);
vector<double> solve_quad(double a, double b, double eps);

/// solve ax+b=0
pair<vector<double>, bool> solve_linear(double a, double b, double eps) {
  vector<double> sol;
  if (abs(a) > eps) {
    sol.push_back(-b / a);
    return make_pair(sol, false);
  } else if (abs(b) > eps) {
    return make_pair(sol, false);
  } else {
    return make_pair(sol, true);
  }
}

/// solve ax^2+bx+c=0
pair<vector<double>, bool> solve_quad(double a, double b, double c,
                                      double eps) {
  if (abs(a) > eps) {
    return make_pair(solve_quad(b / a, c / a, eps), false);
  } else {
    return solve_linear(b, c, eps);
  }
}

/// solve x^2+ax+b=0
vector<double> solve_quad(double a, double b, double eps) {
  double det = a * a - 4 * b;
  vector<double> result;
  if (!(det < 0)) {
    if (det < eps) {
      result.push_back(-a / 2);
    } else {
      const double r = sqrt(det) / 2;
      result.push_back(-a / 2 - r);
      result.push_back(-a / 2 + r);
    }
  }

  return result;
}