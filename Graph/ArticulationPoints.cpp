#include "../Defines.h"

/* START SOLUTION */

// el   : edge list
// disc : discovered time, init with (|V|, -1)
// low  : lowest reachable node, init with (|V|, -1)
// dads : parents array, init with (|V|, -1)
// aps  : list of articulation points, init empty
// bs   : list of bridges, init empty
// n    : current node, init to some unvisited node
// time : current time, init to anything
void artic_f(vvi& el, vi& disc, vi& low, vi& dads, si& aps,
    spii& bs, int n, int& time) {
  disc[n] = low[n] = ++time;

  int kids = 0;
  for (int k : el[n]) {
    if (disc[k] != -1) { // vis check
      if (k != dads[n]) low[n] = min(low[n], disc[k]);
    } else {
      ++kids;
      dads[k] = n;
      artic_f(el, disc, low, dads, aps, bs, k, time);
      low[n] = min(low[n], low[k]);
      
      if (dads[n] != -1 && low[k] >= disc[n]) aps.insert(n);
      if (low[k] > disc[n]) bs.insert({min(n,k), max(n,k)});
    }
  }
  
  if (dads[n] == -1 && kids > 1) aps.insert(n);
} 

/* END SOLUTION */

// http://uva.onlinejudge.org/external/3/315.html
void solve_network() {
  while (true) {
    int N;
    cin >> N >> ws;
    if (N == 0) break;

    vvi el(N, vi());
    while (true) {
      string line;
      getline(cin, line);
      iss ss(line);
      int n;
      ss >> n;
      if (n == 0) break;
      --n;
      int k;
      while (!ss.eof()) {
        ss >> k >> ws;
        --k;
        el[n].push_back(k);
        el[k].push_back(n);
      }
    }

    vi disc(N, -1);
    vi low(N, -1);
    vi dads(N, -1);
    si aps;
    spii bs;
    int time = 0;
    artic_f(el, disc, low, dads, aps, bs, 0, time);

    cout << aps.size() << endl;

  }
}

void solve_critical_links() {
  while (!(cin >> ws).eof()) {
    int N;
    cin >> N >> ws;

    vvi el(N, vi());
    for (int i = 0; i < N; i++) {
      int n, es;
      char d;
      cin >> n >> ws >> d >> es >> d;
      for (int j = 0; j < es; j++) {
        int k;
        cin >> k;
        el[n].push_back(k);
      }
    }

    vi disc(N, -1);
    vi low(N, -1);
    vi dads(N, -1);
    si aps;
    spii bs;
    int time = 0;
    for (int i = 0; i < N; i++) {
      if (disc[i] == -1) {
        artic_f(el, disc, low, dads, aps, bs, i, time);
      }
    }

    cout << bs.size() << " critical links" << endl;
    for (auto& e : bs) {
      cout << e.first << " - " << e.second << endl;
    }
    cout << endl;
  }
}

int main() {
  solve_network();
  //solve_critical_links();
}
