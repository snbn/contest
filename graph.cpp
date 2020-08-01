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

class TopologicalSort {
  const vector<vector<int>> &mOutEdge;
  vector<int> mIsVisited;

 public:
  TopologicalSort(const TopologicalSort &) = delete;
  TopologicalSort &operator=(const TopologicalSort &) = delete;
  TopologicalSort(TopologicalSort &&) = delete;
  TopologicalSort &operator=(TopologicalSort &&) = delete;

  explicit TopologicalSort(const vector<vector<int>> &outEdge)
      : mOutEdge(outEdge), mIsVisited(outEdge.size()) {}
  vector<int> build() {
    const int N = mIsVisited.size();
    fill(mIsVisited.begin(), mIsVisited.end(), false);
    vector<int> sorted;
    for (int i = 0; i < N; i++) {
      if (!mIsVisited[i]) {
        dfs(i, sorted);
      }
    }
    return sorted;
  }

 private:
  void dfs(int node, vector<int> &sorted) {
    mIsVisited[node] = true;
    for (int c : mOutEdge[node]) {
      if (!mIsVisited[c]) {
        dfs(c, sorted);
      }
    }
    sorted.push_back(node);
  }
};

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