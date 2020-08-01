#include <vector>

using namespace std;

class EulerTour {
  const vector<vector<int>> &mEdge;

  vector<int> mOrderL, mOrderR, mSorted;

 public:
  explicit EulerTour(const vector<vector<int>> &edge)
      : mEdge(edge), mOrderL(edge.size()), mOrderR(edge.size()) {
    dfs(0, -1);
  }
  int l(int node) const { return mOrderL[node]; }
  int r(int node) const { return mOrderR[node]; }
  const vector<int> &tour() const { return mSorted; }

 private:
  void dfs(int node, int parent) {
    mOrderL[node] = mSorted.size();
    mSorted.push_back(node);
    for (int c : mEdge[node]) {
      if (c != parent) {
        dfs(c, node);
      }
      mSorted.push_back(node);
    }
    mOrderR[node] = mSorted.size() - 1;
  }
};