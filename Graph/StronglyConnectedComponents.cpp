#include <iostream>
#include <map>
#include <cctype>
#include <stack>
#include <vector>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;

/* START SOLUTION */

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

/* END SOLUTION */

// solves http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=2756
string trim(const string& str) {
  int i = 0, j = str.size()-1;
  while (!isalpha(str[i])) i++;
  while (!isalpha(str[j])) j--;
  return str.substr(i, j+1);
}

int main() {
  while (1) {
    map<string, int> people;
    int P, T;
    cin >> P >> T;
    string name1, name2;
    getline(cin, name1);
    if (P == 0 && T == 0) break;

    for (int i = 0; i < P; ++i) {
      getline(cin, name1);
      name1 = trim(name1);
      people[name1] = i;
    }
    
    vvi adj(P);
    for (int i = 0; i < T; ++i) {
      getline(cin, name1);
      getline(cin, name2);
      name1 = trim(name1);
      name2 = trim(name2);
      adj[people[name1]].push_back(people[name2]);
    }
    
    vvi sccs = scc(adj);
    //for (const auto& scc : sccs) {
      //cerr << "Scc: ";
      //for (auto i : scc) {
        //cerr << i << ' ';
      //}
      //cerr << endl;
    //}

    cout << sccs.size() << '\n';
  }
}
