#include <vector>

using namespace std;

class UFTree {
  vector<int> m_parent;
  vector<int> m_height;
  vector<int> m_size;

 public:
  UFTree(int size) {
    for (int i = 0; i < size; i++) {
      m_parent.push_back(i);
      m_height.push_back(0);
      m_size.push_back(1);
    }
  }

  int root(int node) {
    if (m_parent[node] == node) {
      return node;
    }
    return m_parent[node] = root(m_parent[node]);
  }
  void merge(int n0, int n1) {
    int r0 = root(n0), r1 = root(n1);
    if (r0 == r1) {
      return;
    }
    if (m_height[r0] < m_height[r1]) swap(r0, r1);
    if (m_height[r0] == m_height[r1]) m_height[r0]++;
    m_parent[r1] = r0;
    m_size[r0] += m_size[r1];
  }
  int size(int node) { return m_size[node]; }
};

template <typename W>
class WUFTree {
  vector<int> parent;
  vector<int> height;
  vector<W> diff_weight;

 public:
  WUFTree(int size) {
    for (int i = 0; i < size; i++) {
      parent.push_back(i);
      height.push_back(0);
      diff_weight.push_back(0);
    }
  }
  int root(int node) { return shrink(node).first; }
  void merge(int n0, int n1, W w) {
    int r0 = root(n0), r1 = root(n1);
    W z = w + weight(n0) - weight(n1);
    if (r0 == r1) {
      return;
    }
    if (height[r0] < height[r1]) {
      swap(r0, r1);
      z = -z;
    }
    if (height[r0] == height[r1]) height[r0]++;
    parent[r1] = r0;
    diff_weight[r1] = z;
  }
  W diff(int n0, int n1) { return weight(n1) - weight(n0); }

 protected:
  pair<int, W> shrink(int node) {
    if (parent[node] == node) {
      return make_pair(node, 0);
    }
    auto p = shrink(parent[node]);
    parent[node] = p.first;
    diff_weight[node] += p.second;
    return make_pair(parent[node], diff_weight[node]);
  }
  W weight(int node) { return shrink(node).second; }
};