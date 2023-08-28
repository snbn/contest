#include <algorithm>
#include <vector>

namespace tree {
void euler_tour(const std::vector<std::vector<int>> &outEdges,
                std::vector<int> &result, int currNode, int parent) {
  result.push_back(currNode);
  for (int child : outEdges[currNode]) {
    if (child != parent) {
      euler_tour(outEdges, result, child, currNode);
    }
    result.push_back(currNode);
  }
}
void order_in_euler_tour(const std::vector<int> &tour,
                         std::vector<int> &result) {
  fill(result.begin(), result.end(), -1);
  for (size_t i = 0; i < tour.size(); i++) {
    const int node = tour[i];
    if (result[node] == -1) result[node] = i;
  }
}
}  // namespace tree
