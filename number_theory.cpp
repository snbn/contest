#include <utility>
#include <vector>

using namespace std;

bool is_prime(int n) {
  for (int p = 2; p * p <= n; p++) {
    if (n % p == 0) {
      return false;
    }
  }

  return true;
}

vector<pair<int, int>> factorize(int n) {
  vector<pair<int, int>> result;
  for (int p = 2; p * p <= n; p++) {
    if (is_prime(p)) {
      int cnt = 0;
      while (n % p == 0) {
        n /= p;
        cnt++;
      }
      if (cnt > 0) {
        result.push_back(make_pair(p, cnt));
      }
    }
  }
  if (n != 1) {
    result.push_back(make_pair(n, 1));
  }
  return move(result);
}