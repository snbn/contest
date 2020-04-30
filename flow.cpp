#include <cstdint>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

class FlowNetwork {
  struct Edge {
    int from;
    int to;
    int64_t capacity;
    int64_t flow;
  };

  vector<vector<int>> outEdge, inEdge;
  vector<Edge> edgeData;

 public:
  FlowNetwork(int size) : outEdge(size), inEdge(size) {}

  void addEdge(const int from, const int to, const int64_t capacity) {
    const int eId = edgeData.size();
    Edge eData;
    eData.from = from;
    eData.to = to;
    eData.flow = 0;
    eData.capacity = capacity;
    edgeData.push_back(eData);
    outEdge[from].push_back(eId);
    inEdge[to].push_back(eId);
  }

  int64_t maxflow(const int source, const int sink) {
    const int n = outEdge.size();

    while (true) {
      // check if reachable to sink
      vector<int> parent(n, -1);
      queue<int> q;
      q.push(source);

      while (!q.empty()) {
        const int v = q.front();
        q.pop();

        for (const int e : outEdge[v]) {
          const Edge &data = edgeData[e];
          const int nv = data.to;
          if (parent[nv] < 0 && data.capacity - data.flow > 0) {
            q.push(nv);
            parent[nv] = e;
          }
        }

        for (const int e : inEdge[v]) {
          const Edge &data = edgeData[e];
          const int nv = data.from;
          if (parent[nv] < 0 && data.flow > 0) {
            q.push(nv);
            parent[nv] = e;
          }
        }
      }

      if (parent[sink] < 0) {
        break;
      }

      // select path from source to sink
      int64_t d = numeric_limits<int64_t>::max();
      int v = sink;
      while (v != source) {
        const int e = parent[v];
        const Edge &data = edgeData[e];
        if (data.to == v) {
          d = min(d, data.capacity - data.flow);
          v = data.from;
        } else {
          d = min(d, data.flow);
          v = data.to;
        }
      }

      // update flow on the path
      v = sink;
      while (v != source) {
        const int e = parent[v];
        Edge &data = edgeData[e];
        if (data.to == v) {
          data.flow += d;
          v = data.from;
        } else {
          data.flow -= d;
          v = data.to;
        }
      }
    }

    int64_t result = 0;
    for (const int e : inEdge[sink]) {
      result += edgeData[e].flow;
    }
    return result;
  }
};