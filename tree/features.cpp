#include <queue>
#include <vector>

using namespace std;

vector<int> tree_size(int root, const vector<vector<int>> &edge) {
  vector<int> result(edge.size(), 1);
  vector<int> idx(edge.size(), 0);
  vector<int> parent(edge.size(), -1);
  vector<int> s;
  s.push_back(root);
  while (!s.empty()) {
    int node = s.back();
    int &i = idx[node];
    if (i < edge[node].size()) {
      if (edge[node][i] == parent[node]) {
        i++;
        continue;
      }
      s.push_back(edge[node][i]);
      parent[edge[node][i]] = node;
      i++;
    } else {
      for (int c : edge[node]) {
        if (c != parent[node]) {
          result[node] += result[c];
        }
      }
      s.pop_back();
    }
  }
  return result;
}

vector<int> tree_parent(int root, const vector<vector<int>> &edge) {
  vector<int> result(edge.size(), -1);
  queue<int> q;
  q.push(root);

  while (!q.empty()) {
    const int node = q.front();
    q.pop();

    for (int c : edge[node]) {
      if (c != result[node]) {
        result[c] = node;
        q.push(c);
      }
    }
  }
  return result;
}

vector<int> tree_depth(int root, const vector<vector<int>> &edge) {
  queue<int> q;
  vector<int> depth(edge.size(), -1);

  q.push(root);
  depth[root] = 0;
  while (!q.empty()) {
    int v = q.front();
    q.pop();

    for (int c : edge[v]) {
      if (depth[c] < 0) {
        depth[c] = depth[v] + 1;
        q.push(c);
      }
    }
  }
  return depth;
}