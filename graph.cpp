#include <algorithm>
#include <vector>

void topological_sort(int node, const std::vector<std::vector<int>> &outEdge,
                      std::vector<int> &sorted, std::vector<bool> &isVisited) {
  if (isVisited[node]) return;
  isVisited[node] = true;
  for (int c : outEdge[node]) {
    topological_sort(c, outEdge, sorted, isVisited);
  }
  sorted.push_back(node);
}

void sort_preorder(const int node,
                   const std::vector<std::vector<int>> &outEdgeList,
                   std::vector<int> &sorted, std::vector<bool> &isVisited) {
  if (isVisited[node]) return;
  isVisited[node] = true;

  for (const int c : outEdgeList[node]) {
    sort_preorder(c, outEdgeList, sorted, isVisited);
  }

  sorted.push_back(node);
}

std::vector<std::vector<int>> inversed_edge(
    const std::vector<std::vector<int>> &edgeList) {
  std::vector<std::vector<int>> result(edgeList.size());

  for (int to = 0; to < edgeList.size(); to++) {
    for (int from : edgeList[to]) {
      result[from].push_back(to);
    }
  }
  return result;
}

void assign_group(const int node, const int group,
                  const std::vector<std::vector<int>> &inEdgeList,
                  std::vector<int> &groupList, std::vector<bool> &isVisited) {
  if (isVisited[node]) return;
  isVisited[node] = true;

  groupList[node] = group;
  for (const int c : inEdgeList[node]) {
    assign_group(c, group, inEdgeList, groupList, isVisited);
  }
}

std::vector<int> scc(const std::vector<std::vector<int>> &outEdgeList) {
  const int N = outEdgeList.size();
  std::vector<int> preordered;
  {
    std::vector<bool> isVisited(N, false);
    for (int node = 0; node < N; node++) {
      sort_preorder(node, outEdgeList, preordered, isVisited);
    }
  }

  std::reverse(preordered.begin(), preordered.end());
  auto inEdgeList = inversed_edge(outEdgeList);

  std::vector<int> groupList(N, -1);
  {
    std::vector<bool> isVisited(N, false);
    for (const int node : preordered) {
      assign_group(node, node, inEdgeList, groupList, isVisited);
    }
  }

  return groupList;
}