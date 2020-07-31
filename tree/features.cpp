#include <queue>
#include <vector>

using namespace std;

void tree_size(int node, int parent, const vector<vector<int>> &edge,
               vector<int> &size) {
  size[node] = 1;
  for (int c : edge[node]) {
    if (c != parent) {
      tree_size(c, node, edge, size);
      size[node] += size[c];
    }
  }
}

void tree_parent(int node, int parent, const vector<vector<int>> &edge,
                 vector<int> &parents) {
  parents[node] = parent;
  for (int c : edge[node]) {
    if (c != parent) {
      tree_parent(c, node, edge, parents);
    }
  }
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