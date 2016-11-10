/* START SOLUTION */
// Finds and euler path or cycle.
// Is a path/cycle that visits every edge.
// Undirected graph has eulerian cycle iff every vertex has even degree.
// Undirected graph has eulerian path iff 0 or 2 vertiex have odd degree.
// Directed has cycle iff in degree = out degree for every vertex + is SCC.
// Directed has path iff <= 1 vertex has (in degree)-(out degree) = 1
// and <= 1 vertex has out-in = 1, and the others have in=out.
struct EulerianGraph {
  vector<int> path; // Will be in reverse order.
  // adj will not all be 0 if the graph has > 1 component with > 1 vertex
  // and thus no cycle/path.
  vector<vector<int>> adj; // Could be vector<multiset<>> for sparse graphs
  void hierholtz(int at = 0) {
    // Iterate through children and remove/mark undirected edge.
    for (int c = 0; c < adj[at].size(); ++c) if (adj[at][c]) {
      --adj[at][c];
      //--adj[c][at]; uncomment this for undirected graph
      hierholtz(c);
    }
    path.push_back(at);
  }
};
/* END SOLUTION */
