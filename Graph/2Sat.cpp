#include "../Defines.h"

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
bool two_sat(int num_vars, const vpii& impls,
    vb& assignment, vb& vars_set) {
  vvi adj(2 * num_vars);
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
  int num_vars = 3;
  vector<pair<int, int>> impls = {{0, 2}, {3, 4}, {4, 0}, {0, 3}};
  vector<bool> assign(2 * num_vars, false);
  vector<bool> set_vars(2 * num_vars, false);

  assign[0] = true;
  assign[1] = false;
  set_vars[0] = set_vars[1] = true;

  if (two_sat(num_vars, impls, assign, set_vars)) printf("Assignable\n");
  else printf("Not assignable\n");
  for (int i = 0; i < num_vars; ++i) {
    printf("%d: ", i);
    if (assign[2 * i]) {
      printf("true\n");
    } else {
      printf("false\n");
    }
  }
}

int solve_buglife() {
    int S;
    cin >> S;

    for(int s = 1; s <= S; ++s) {
        ll B, I;
        cin >> B >> I;

        vpii imps;
        vb val(B), ass(B);

        for(int i = 0; i < I; ++i) {
            int a, b;
            cin >> a >> b;
            --a; --b;

            imps.push_back({2*a, 2*b+1});
            imps.push_back({2*b, 2*a+1});
        }

        cout << "Scenario #" << s << ":\n";
        if(!two_sat(B, imps, ass, val))
            cout << "No suspicious bugs found!\n";
        else cout << "Suspicious bugs found!\n"; 
    }
    return 0;
}

int main() {
  freopen("mountain.in", "r", stdin); freopen("mountain.out", "w", stdout);

  int T, N;
  scanf("%d %d", &T, &N);

  vpii impls;
  for (int n = 0; n < N; ++n) {
    int i, j;
    char X, Y;
    scanf("%d %c %d %c ", &i, &X, &j, &Y);
    i--;
    j--;
    int ivar = 2 * i;
    if (X == 'R') ivar++;
    int jvar = 2 * j;
    if (Y == 'R') jvar++;
    impls.push_back({not_var(ivar), jvar});
    impls.push_back({not_var(jvar), ivar});
  }
  
  vb assignments(2 * T, false);
  vb var_set(2 * T, false);
  if (two_sat(T, impls, assignments, var_set)) {
    for (int i = 0; i < T; ++i) {
      if (assignments[2*i + 1]) {
        printf("R");
      } else {
        printf("L");
      }
    }
    printf("\n");
  } else {
    printf("LOST\n");
  }
}
