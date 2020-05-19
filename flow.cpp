#include <cstdint>
#include <limits>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

class FlowNetwork {
  struct Edge {
    int to;
    int64_t capacity;
  };

  const int source, sink;
  int64_t flowCache;
  vector<vector<size_t>> outEdge;
  vector<Edge> edgeData;

 public:
  FlowNetwork(int size, int source, int sink)
      : source(source), sink(sink), flowCache(0), outEdge(size) {}

  void addEdge(const int from, const int to, const int64_t capacity) {
    const size_t eId = edgeData.size();
    Edge eData1{to, capacity};
    edgeData.push_back(eData1);
    outEdge[from].push_back(eId);

    Edge eData2{from, 0};
    edgeData.push_back(eData2);
    outEdge[to].push_back(eId + 1);
  }

  void fordFulkerson() {
    const size_t n = outEdge.size();

    while (true) {
      // check if reachable to sink
      vector<int> parent(n, -1);
      queue<size_t> q;
      q.push(source);

      while (!q.empty()) {
        const size_t v = q.front();
        q.pop();

        for (const size_t eId : outEdge[v]) {
          const Edge &data = edgeData[eId];
          const size_t nv = data.to;
          if (parent[nv] < 0 && data.capacity > 0) {
            q.push(nv);
            parent[nv] = eId;
          }
        }
      }

      if (parent[sink] < 0) {
        break;
      }

      // select path from source to sink
      int64_t f = numeric_limits<int64_t>::max();
      int v = sink;
      while (v != source) {
        const int eId = parent[v];
        f = min(f, edgeData[eId].capacity);
        v = edgeData[eId ^ 1].to;
      }

      // update flow on the path
      v = sink;
      while (v != source) {
        const int eId = parent[v];
        edgeData[eId].capacity -= f;
        edgeData[eId ^ 1].capacity += f;
        v = edgeData[eId ^ 1].to;
      }
      flowCache += f;
    }
  }

  int64_t maxflow() const { return flowCache; }
};

class WeightedFlowNetwork {
  struct Edge {
    int from;
    int to;
    int64_t capacity;
    int64_t flow;
    int64_t cost;
  };

  vector<vector<int>> outEdge, inEdge;
  vector<Edge> edgeData;
  vector<int> potential;

 public:
  WeightedFlowNetwork(const int size)
      : outEdge(size), inEdge(size), potential(size, 0) {}

  void addEdge(const int from, const int to, const int64_t capacity,
               const int64_t cost) {
    const int eId = edgeData.size();
    Edge eData;
    eData.from = from;
    eData.to = to;
    eData.flow = 0;
    eData.capacity = capacity;
    eData.cost = cost;
    edgeData.push_back(eData);
    outEdge[from].push_back(eId);
    inEdge[to].push_back(eId);
  }

  /// returns (|flow|, mincost)
  pair<int64_t, int64_t> minCostFlow(const int source, const int sink,
                                     const int64_t f) {
    const int n = outEdge.size();
    int64_t demand = f;

    while (true) {
      // check if reachable to sink
      const int64_t INF = 1'000'000'000;
      using P = pair<int64_t, int>;
      vector<int> parent(n, -1);
      vector<int64_t> dist(n, INF);
      priority_queue<P, vector<P>, greater<P>> q;

      dist[source] = 0;
      q.push(P(0, source));

      while (!q.empty()) {
        int64_t d;
        int v;
        tie(d, v) = q.top();
        q.pop();

        if (d > dist[v]) {
          continue;
        }

        for (const int e : outEdge[v]) {
          const Edge &data = edgeData[e];
          const int nv = data.to;
          const int64_t nd = d + data.cost + potential[v] - potential[nv];
          if (dist[nv] > nd && data.capacity - data.flow > 0) {
            q.push(P(nd, nv));
            dist[nv] = nd;
            parent[nv] = e;
          }
        }

        for (const int e : inEdge[v]) {
          const Edge &data = edgeData[e];
          const int nv = data.from;
          const int64_t nd = d - data.cost + potential[v] - potential[nv];
          if (dist[nv] > nd && data.flow > 0) {
            q.push(P(nd, nv));
            dist[nv] = nd;
            parent[nv] = e;
          }
        }
      }

      for (int i = 0; i < potential.size(); i++) potential[i] += dist[i];

      if (parent[sink] < 0 || demand == 0) {
        break;
      }

      // select path from source to sink
      int64_t fDiff = demand;
      int v = sink;
      while (v != source) {
        const int e = parent[v];
        const Edge &data = edgeData[e];
        if (data.to == v) {
          fDiff = min(fDiff, data.capacity - data.flow);
          v = data.from;
        } else {
          fDiff = min(fDiff, data.flow);
          v = data.to;
        }
      }

      // update flow on the path
      v = sink;
      while (v != source) {
        const int e = parent[v];
        Edge &data = edgeData[e];
        if (data.to == v) {
          data.flow += fDiff;
          v = data.from;
        } else {
          data.flow -= fDiff;
          v = data.to;
        }
      }

      demand -= fDiff;
    }

    int64_t rCost = 0;
    for (const Edge &eData : edgeData) {
      rCost += eData.cost * eData.flow;
    }
    return pair<int64_t, int64_t>(f - demand, rCost);
  }
};