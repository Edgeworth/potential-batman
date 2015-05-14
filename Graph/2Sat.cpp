#include "Defines.h"

struct Node {
  Node() : low(-1), index(-1) {}
  int low, index;
};

// Computes some SCCs using Tarjan's. DON'T USE THIS ONE.
// n: Node to start with.
// adj: Adjacency list.
// index: Current timestamp for a node.
// s: stack
// cur_scc: like a seen array
// sccs: output sccs.
void scc_internal(
    int n, 
    const vvi& adj, 
    int& index,
    vector<Node>& nodes,
    stack<int>& s,
    vector<bool>& cur_scc, 
    vvi& sccs) {
  nodes[n].index = nodes[n].low = index++;
  cur_scc[n] = true;
  s.push(n);

  for (auto i : adj[n]) {
    if (nodes[i].index == -1) {
      scc_internal(i, adj, index, nodes, s, cur_scc, sccs);
      nodes[n].low = min(nodes[n].low, nodes[i].low);
    } else if (cur_scc[i]) {
      nodes[n].low = min(nodes[n].low, nodes[i].index);
    }
  }

  if (nodes[n].low == nodes[n].index) {
    sccs.emplace_back();
    while (1) {
      int k = s.top();
      sccs.back().push_back(k);
      s.pop();
      cur_scc[k] = false;
      if (k == n) break;
    }
  }
}

// USE THIS ONE.
vvi scc(const vvi& adj) {
  vvi sccs;
  int index = 0;
  vector<Node> nodes(adj.size());
  stack<int> s;
  for (int i = 0; i < adj.size(); ++i) {
    if (nodes[i].index == -1) {
      vector<bool> cur_scc(adj.size(), false);
      scc_internal(i, adj, index, nodes, s, cur_scc, sccs);
    }
  }

  return sccs;
}

/* START SOLUTION */
#include <unordered_set>

inline int not_var(int var) {
  return var + (1 - 2 * (var % 2));
}

// Returns assignment in assignment.
// Make sure assignment[i] = !assignment[not_var(i)].
// impls: Use 2 * var for positive and 2 * var + 1 for not var.
// 2i = I
// 2i + 1 = not I
bool two_sat(int numVars, 
    const vector<pair<int, int>>& impls,
    vector<bool>& assignment, 
    vector<bool>& vars_set) {
  vvi adj(2 * numVars);
  for (const auto& impl : impls) {
    adj[impl.first].push_back(impl.second);
    adj[not_var(impl.second)].push_back(not_var(impl.first));
  }

  vvi components = scc(adj);
  for (const auto& component : components) {
    unordered_set<int> included_vars;
    for (int var : component) {
      int positive = var & (~1);
      if (included_vars.count(positive)) {
        return false;
      }
      included_vars.insert(var & (~1));
    }
  }

  for (const auto& component : components) {
    bool found_set = false;
    bool to_set = true;
    for (int var : component) {
      if (vars_set[var]) {
        if (found_set && (assignment[var] != to_set ||
            assignment[not_var(var)] != !to_set)) {
          return false;
        }
        found_set = true;
        to_set = assignment[var];
      }
    }
    for (int var : component) {
      vars_set[var] = true;
      assignment[var] = to_set;
      vars_set[not_var(var)] = true;
      assignment[not_var(var)] = !to_set;
    }
  }

  return true;
}

/* END SOLUTION */

void test() {
  int numVars = 3;
  vector<pair<int, int>> impls = {{0, 2}, {3, 4}, {4, 0}, {0, 3}};
  vector<bool> assign(2 * numVars, false);
  vector<bool> set_vars(2 * numVars, false);

  assign[0] = true;
  assign[1] = false;
  set_vars[0] = set_vars[1] = true;

  if (two_sat(numVars, impls, assign, set_vars)) printf("Assignable\n");
  else printf("Not assignable\n");
  for (int i = 0; i < numVars; ++i) {
    printf("%d: ", i);
    if (assign[2 * i]) {
      printf("true\n");
    } else {
      printf("false\n");
    }
  }
}

int main() {
  test();
}
