#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <tuple>
#include <vector>

using namespace std;

#define rep(i, n) for (int64_t i = 0; i < (n); i++)
#define irep(i, n) for (int64_t i = 0; i <= (n); i++)
#define rrep(i, n) for (int64_t i = (n)-1; i >= 0; i--)
#define rirep(i, n) for (int64_t i = n; i >= 0; i--)

template <typename F>
void forComb(int n, int k, F f)
{
    int comb = (1 << k) - 1;
    while (comb < (1 << n)) {
        f(comb);

        int x = comb & -comb;
        int y = comb + x;
        comb = ((comb & ~y) / x >> 1) | y;
    }
}

int64_t dceil(int64_t, int64_t);
int64_t dfloor(int64_t, int64_t);

int64_t dceil(int64_t x, int64_t y)
{
    if (y < 0) {
        return dceil(-x, -y);
    }
    if (x < 0) {
        return -dfloor(-x, y);
    }
    return (x + y - 1) / y;
}

int64_t dfloor(int64_t x, int64_t y)
{
    if (y < 0) {
        return dfloor(-x, -y);
    }
    if (x < 0) {
        return -dceil(-x, y);
    }
    return x / y;
}

template <typename F>
void forNeighbor4(int x, int y, F f)
{
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (abs(dx) + abs(dy) == 1) {
                f(x + dx, y + dy);
            }
        }
    }
}

const uint64_t MOD = 1'000'000'007L;

uint64_t dpow(uint64_t x, uint64_t y)
{
    if (y == 0) {
        return 1;
    }
    if (y % 2 == 0) {
        return dpow(x * x % MOD, y / 2);
    } else {
        return x * dpow(x, y - 1) % MOD;
    }
}

uint64_t inv(uint64_t x)
{
    return dpow(x, MOD - 2);
}

uint64_t comb(uint64_t n, uint64_t k)
{
    uint64_t a = 1, b = 1;
    for (uint64_t i = 0; i < k; i++) {
        a *= (n - i);
        a %= MOD;
        b *= i + 1;
        b %= MOD;
    }
    return a * inv(b) % MOD;
}

uint64_t perm(uint64_t n, uint64_t k)
{
    uint64_t r = 1;
    for (uint64_t i = 0; i < k; i++) {
        r *= (n - i);
        r %= MOD;
    }
    return r;
}

class UFTree {
    vector<int> parent;
    vector<int> height;

public:
    UFTree(int size)
    {
        for (int i = 0; i < size; i++) {
            parent.push_back(i);
            height.push_back(0);
        }
    }
    int root(int node)
    {
        if (parent[node] == node) {
            return node;
        }
        return parent[node] = root(parent[node]);
    }
    void merge(int n0, int n1)
    {
        int r0 = root(n0), r1 = root(n1);
        if (r0 == r1) {
            return;
        }
        if (height[r0] < height[r1])
            swap(r0, r1);
        if (height[r0] == height[r1])
            height[r0]++;
        parent[r1] = r0;
    }
};

template <typename W>
class WUFTree {
    vector<int> parent;
    vector<int> height;
    vector<W> diff_weight;

public:
    WUFTree(int size)
    {
        for (int i = 0; i < size; i++) {
            parent.push_back(i);
            height.push_back(0);
            diff_weight.push_back(0);
        }
    }
    int root(int node)
    {
        return shrink(node).first;
    }
    void merge(int n0, int n1, W w)
    {
        int r0 = root(n0), r1 = root(n1);
        W z = w + weight(n0) - weight(n1);
        if (r0 == r1) {
            return;
        }
        if (height[r0] < height[r1]) {
            swap(r0, r1);
            z = -z;
        }
        if (height[r0] == height[r1])
            height[r0]++;
        parent[r1] = r0;
        diff_weight[r1] = z;
    }
    W diff(int n0, int n1)
    {
        return weight(n1) - weight(n0);
    }

protected:
    pair<int, W> shrink(int node)
    {
        if (parent[node] == node) {
            return make_pair(node, 0);
        }
        auto p = shrink(parent[node]);
        parent[node] = p.first;
        diff_weight[node] += p.second;
        return make_pair(parent[node], diff_weight[node]);
    }
    W weight(int node)
    {
        return shrink(node).second;
    }
};

template <typename T>
class SegmentTree {
    const T unit;
    const function<T(const T&, const T&)> op;
    vector<T> container;

public:
    template <typename Op>
    SegmentTree(int size, const T& identity, const Op& operation)
        : unit(identity)
        , op(operation)
        , container(msb(size * 2 - 1))
    {
        fill(container.begin(), container.end(), unit);
    }
    void update(size_t index, T value)
    {
        int n = container.size() / 2;
        container[index + n] = value;

        int node = (index + n) >> 1;
        while (node > 0) {
            container[node] = op(container[node << 1], container[(node << 1) + 1]);
            node >>= 1;
        }
    }
    T find(size_t s, size_t t)
    {
        if (s >= t) {
            return unit;
        }
        return find_inner(s, t, 0, container.size() / 2);
    }

private:
    T find_inner(size_t s, size_t t, size_t lb, size_t ub)
    {
        if (s == lb && t == ub) {
            int n = container.size() / 2;
            return container[lca(n + s, n + t - 1)];
        }

        T result = unit;
        size_t mid = (lb + ub) / 2;
        if (mid > s) {
            result = op(result, find_inner(s, min(mid, t), lb, mid));
        }
        if (mid < t) {
            result = op(result, find_inner(max(s, mid), t, mid, ub));
        }

        return result;
    }
    int lca(int n0, int n1)
    {
        int a = n0 ^ n1;
        return n0 / msb(a);
    }
    int msb(int x)
    {
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;

        return x + 1;
    }
};

int main()
{
    return 0;
}