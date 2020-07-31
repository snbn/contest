#include <vector>

using namespace std;

class HLDecomposition {
  const vector<vector<int>> &mEdge;
  const vector<int> &mSize;

  using Rel = pair<int, int>;
  vector<Rel> mRelation;
  vector<int> mRoot;
  vector<int> mPosition;

 public:
  explicit HLDecomposition(const vector<vector<int>> &edge,
                           const vector<int> &size)
      : mEdge(edge), mSize(size), mRoot(edge.size()), mPosition(edge.size()) {
    dfs(0, -1, 0);
    stable_sort(
        mRelation.begin(), mRelation.end(),
        [](const Rel &lh, const Rel &rh) { return lh.first < rh.first; });

    for (int i = 0; i < mRelation.size(); i++) {
      mRoot[mRelation[i].second] = mRelation[i].first;
      mPosition[mRelation[i].second] = i;
    }
  }
  int root(int node) { return mRoot[node]; }
  int pos(int node) { return mPosition[node]; }

 private:
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