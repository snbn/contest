#include <vector>

using namespace std;

template <typename It>
vector<int> zAlgorithm(It s, It t) {
  int n = t - s;
  vector<int> z(n);
  if (n == 0) {
    return z;
  }

  int l = 0;
  z[0] = 0;
  for (int i = 1; i < n; i++) {
    int r = l + z[l];

    if (r > i + z[i - l]) {
      z[i] = z[i - l];
    } else {
      z[i] = max(0, r - i);
      l = i;
      for (int j = z[i]; i + j < n && s[i + j] == s[j]; j++) {
        z[i]++;
      }
    }
  }
  z[0] = n;

  return move(z);
}