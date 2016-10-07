// From Max.

/* START SOLUTION */
typedef int cap_t;
const cap_t CAPINF = numeric_limits<cap_t>::max()/2;
// Dinics algorithm.O(V^2E) max flow. O(sqrt(V)*E) for bipartite.
// O(sqrt(V)*E) for any graph in which non-soruce/sink nodes
// Have either 1 incoming or 1 outgoing edges with unit capacity.
// Very fast in practice because I blended some ideas from push-relabel.
struct Dinic {
  int S, T;
  vector<int> layer, dfs_child_i;
  vector<vector<int>> adj;
  vector<vector<cap_t>> cap;
  cap_t dfs(int u, cap_t f) {
    if (u == T) return f;
    for (;dfs_child_i[u] < adj[u].size(); ++dfs_child_i[u]) {
      int v = adj[u][dfs_child_i[u]];
      if (layer[v] != layer[u]+1 || cap[u][v] <= 0) continue;
      cap_t pushedf = dfs(v, min(f, cap[u][v]));
      if (pushedf) {
        cap[u][v] -= pushedf; cap[v][u] += pushedf;
        return pushedf;
      }
    }
    return 0;
  }
  cap_t max_flow(const vector<vector<cap_t>> &incaps, int s, int t) {
    S = s; T = t;
    cap = incaps;
    const int N = cap.size();
    adj.assign(N, vector<int>());
    for (int i = 0; i < N; ++i) { // Build adjacency lists.
      for (int j = 0; j < N; ++j) if (cap[i][j] > 0) {
        adj[i].push_back(j);
        adj[j].push_back(i);
      }
    }
    cap_t flow = 0;
    while (true) {
      layer.assign(N, -1); // Shortest path tree level.
      queue<int> Q; Q.push(S); layer[S] = 0;
      while (!Q.empty()) { // BFS.
        int u = Q.front(); Q.pop();
        for (int c : adj[u]) if (cap[u][c] > 0 && layer[c] == -1) {
          layer[c] = layer[u]+1; Q.push(c);
        }
      }
      if (layer[T]  == -1) break;
      dfs_child_i.assign(N, 0);
      for (cap_t f = dfs(S, CAPINF); f > 0; f = dfs(S, CAPINF)) {
        flow += f;
      }
    }
    return flow;
  }
};
/* END SOLUTION */
