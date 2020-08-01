#include <vector>

using namespace std;

class HLDecomposition {
  const vector<vector<int>> &mEdge;

  vector<int> mSize, mParent, mDepth;

  using Rel = pair<int, int>;
  vector<Rel> mRelation;
  vector<int> mRoot;
  vector<int> mPosition;

 public:
  explicit HLDecomposition(const vector<vector<int>> &edge)
      : mEdge(edge),
        mSize(edge.size(), 1),
        mParent(edge.size(), -1),
        mDepth(edge.size(), 0),
        mRoot(edge.size()),
        mPosition(edge.size()) {
    before_dfs(0);
    dfs(0, -1, 0);
    stable_sort(
        mRelation.begin(), mRelation.end(),
        [](const Rel &lh, const Rel &rh) { return lh.first < rh.first; });

    for (int i = 0; i < mRelation.size(); i++) {
      mRoot[mRelation[i].second] = mRelation[i].first;
      mPosition[mRelation[i].second] = i;
    }
  }
  int parent(int node) { return mParent[node]; }
  int depth(int node) { return mDepth[node]; }
  int root(int node) { return mRoot[node]; }
  int pos(int node) { return mPosition[node]; }
  int lca(int u, int v) {
    while (root(u) != root(v)) {
      if (depth(root(u)) < depth(root(v))) {
        swap(u, v);
      }
      u = parent(root(u));
    }
    return depth(u) < depth(v) ? u : v;
  }
  template <typename F>
  void query(int u, int v, const F &f) {
    const int anscestor = lca(u, v);
    for (int i = 0; i < 2; i++) {
      while (root(u) != root(anscestor)) {
        f(pos(root(u)), pos(u) + 1, (bool)i);
        u = parent(root(u));
      }
      f(pos(anscestor) + 1, pos(u) + 1, (bool)i);
      u = v;
    }
  }

 private:
  void before_dfs(int root) {
    vector<int> idx(mEdge.size(), 0);
    vector<int> s;
    s.push_back(root);
    while (!s.empty()) {
      int node = s.back();
      int &i = idx[node];
      if (i < mEdge[node].size()) {
        if (mEdge[node][i] == mParent[node]) {
          i++;
          continue;
        }
        s.push_back(mEdge[node][i]);
        mDepth[mEdge[node][i]] = mDepth[node] + 1;
        mParent[mEdge[node][i]] = node;
        i++;
      } else {
        for (int c : mEdge[node]) {
          if (c != mParent[node]) {
            mSize[node] += mSize[c];
          }
        }
        s.pop_back();
      }
    }
  }
  void dfs(int node, int parent, int group) {
    mRelation.push_back(Rel(group, node));
    int s = mEdge[node].size() - (parent < 0 ? 0 : 1);
    bool exists = false;
    for (int c : mEdge[node]) {
      if (c != parent) {
        int g = c;
        if (!exists && mSize[c] >= (mSize[node] - 1 + s - 1) / s) {
          g = group;
          exists = true;
        }
        dfs(c, node, g);
      }
    }
  }
};