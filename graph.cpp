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

  for (std::size_t to = 0; to < edgeList.size(); to++) {
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

std::vector<std::vector<int>> merge_scc(
    const std::vector<std::vector<int>> &outEdgeList,
    const std::vector<int> &groupList) {
  std::vector<std::vector<int>> result(outEdgeList.size());

  for (std::size_t s = 0; s < outEdgeList.size(); s++) {
    const int from = groupList[s];
    for (int t : outEdgeList[s]) {
      const int to = groupList[t];
      if (from != to) {
        result[from].push_back(to);
      }
    }
  }

  for (auto &list : result) {
    std::sort(list.begin(), list.end());
    list.erase(std::unique(list.begin(), list.end()), list.end());
  }

  return result;
}