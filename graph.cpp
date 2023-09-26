#include <vector>

using namespace std;

vector<vector<int>> inversed_edge(const vector<vector<int>> &edge) {
  vector<vector<int>> result(edge.size());
  for (int i = 0; i < edge.size(); i++) {
    for (int from : edge[i]) {
      result[from].push_back(i);
    }
  }
  return result;
}

void topological_sort(int node, const std::vector<std::vector<int>> &outEdge,
                      std::vector<int> &sorted, std::vector<bool> &isVisited) {
  if (isVisited[node]) return;
  isVisited[node] = true;
  for (int c : outEdge[node]) {
    topological_sort(c, outEdge, sorted, isVisited);
  }
  sorted.push_back(node);
}

class StrictlyConnectedComponent {
  const vector<vector<int>> &mInEdge;
  const vector<int> mSorted;
  vector<int> mIsVisited;

 public:
  StrictlyConnectedComponent(const StrictlyConnectedComponent &) = delete;
  StrictlyConnectedComponent &operator=(const StrictlyConnectedComponent &) =
      delete;
  StrictlyConnectedComponent(StrictlyConnectedComponent &&) = delete;
  StrictlyConnectedComponent &operator=(StrictlyConnectedComponent &&) = delete;

  explicit StrictlyConnectedComponent(const vector<vector<int>> &inEdge,
                                      const vector<int> &sorted)
      : mInEdge(inEdge), mSorted(sorted), mIsVisited(inEdge.size()) {}
  vector<int> build() {
    const int N = mIsVisited.size();
    vector<int> group(N);
    fill(mIsVisited.begin(), mIsVisited.end(), false);
    for (int i = N - 1; i >= 0; i--) {
      if (!mIsVisited[mSorted[i]]) {
        dfs(mSorted[i], i, group);
      }
    }
    return group;
  }

 private:
  void dfs(int node, int curr, vector<int> &group) {
    mIsVisited[node] = true;
    group[node] = curr;
    for (int c : mInEdge[node]) {
      if (!mIsVisited[c]) {
        dfs(c, curr, group);
      }
    }
  }
};