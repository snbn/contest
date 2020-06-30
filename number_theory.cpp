#include <tuple>
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

vector<size_t> factors(size_t n) {
  vector<size_t> result;
  for (size_t d = 1; d * d <= n; d++) {
    if (n % d == 0) {
      result.push_back(d);
      if (n / d != d) {
        result.push_back(n / d);
      }
    }
  }
  return result;
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

vector<bool> eratosthenes_sieve(size_t size) {
  vector<bool> is_prime(size + 1, true);
  for (size_t i = 0; i <= size; i++) {
    if (i < 2) {
      is_prime[i] = false;
    } else if (is_prime[i]) {
      for (size_t j = 2 * i; j <= size; j += i) {
        is_prime[j] = false;
      }
    }
  }
  return move(is_prime);
}

int totient(const int n) {
  vector<pair<int, int>> fs = factorize(n);
  int result = n;

  for (auto pa : fs) {
    int p, e;
    tie(p, e) = pa;
    result /= p;
    result *= p - 1;
  }
  return result;
}