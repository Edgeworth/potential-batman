#include "../Defines.h"

/* START SOLUTION */

// sets are indexed between 0 and N-1 inclusive

struct ufs {
  vector<int> parents;
  vector<int> ranks;

  ufs(int N) : parents(N, -1), ranks(N) {};

  // returns a representative of i's set
  int find(int i) {
    if(parents[i] == -1) return i;

    parents[i] = find(parents[i]);
    return parents[i];
  }

  // union sets including i and j
  void unio(int i, int j) {
    int ip = find(i), jp = find(j);

    if(ip == jp) return;

    if(ranks[ip] > ranks[jp]) {
      parents[jp] = ip;
    } else { // ip's rank <= jp's rank
      parents[ip] = jp;
      ranks[jp] = max(ranks[jp], ranks[ip]+1);
    }
  }
};

/* END SOLUTION */

// solves http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&category=359&problem=400
int solve_graph_con() {
  string line;
  int T;
  cin >> T;
  getline(cin, line);
  getline(cin, line);

  for(int t = 0; t < T; ++t) {
    getline(cin, line);
    int N = line[0] - 'A' + 1;
     
    ufs uf(N);
    while(true) {
      getline(cin, line);
      if(line.size() == 0) break;

      int u = line[0] - 'A', v = line[1] - 'A';
      uf.unio(u, v);
    }

    set<int> uniques;
    for(int i = 0; i < N; ++i) uniques.insert(uf.find(i));

    if(t != 0) cout << endl;
    cout << uniques.size() << endl;
  }
  return 0;
}

// solves http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&category=359&problem=2737
int solve_money_matters() {
  int T;
  cin >> T;

  for(int t = 0; t < T; ++t) {
    int n, m;
    cin >> n >> m;

    ufs uf(n);

    vector<int> owed(n, 0);
    for(int i = 0; i < n; ++i) cin >> owed[i];

    for(int j = 0; j < m; ++j) {
      int u, v;
      cin >> u >> v;
      uf.unio(u, v);
    }

    vector<int> tots(n);
    for(int i = 0; i < n; ++i) tots[uf.find(i)] += owed[i];

    bool all0 = true;
    for(int i = 0; i < n; ++i) all0 = all0 && tots[i] == 0;

    cout << (all0 ? "POSSIBLE" : "IMPOSSIBLE") << endl;
  }
  return 0;
}

int main() {
  return solve_money_matters();
}
