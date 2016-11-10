// From Max.
/* START SOLUTION */
struct LCA { // Lowest common ancestor in tree
    vector<pair<int, int>> order; // Preorder in dfs
    vector<int> fst; // First occurance in order
    vector<vector<int>> adj; // adj lists
    RangeTree<pair<int, int>> rt; // RangeMinQuery tree
    void dfs(int at, int p, int depth) {
        fst[at] = order.size(); order.push_back({depth, at});
        for (int c : adj[at]) if (c != p) {
            dfs(c, at, depth+1); order.push_back({depth, at});
        }
    }
    int query(int a, int b) {
        if (fst[a] > fst[b]) swap(a, b);
        return rt.query(fst[a], fst[b]).second;
    }
  int query(int root, int a, int b) {
    int aa = query(a,b), bb = query(root,b), cc = query(a,root);
    if (aa == bb && bb != cc)
      return cc+1;
    else if (aa == cc && bb != cc)
      return bb+1;
    else if (cc==bb && bb != aa)
      return aa+1;
    else
      return aa+1;
  }
    LCA(const vector<vector<int>> &adj) : adj(adj),
        rt(adj.size()*2, {1e9, -1}), fst(adj.size()) {
        dfs(0, 0, 0);
        for (int i = 0; i < order.size(); ++i) { rt.update(i, order[i]); }
    }
};
/* END SOLUTION */
