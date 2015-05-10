#include "../Defines.h"

int d = 0;
void pd() {
  for (int i = 0; i < d; ++i) cerr << "\t";
}

// el   : edge list
// disc : discovered time, init with (|V|, -1)
// low  : lowest reachable node, init with (|V|, -1)
// dads : parents array, init with (|V|, -1)
// aps  : list of articulation points, init empty
// n    : current node
// time : current time, init 0
// returns the highest time seen
int artic_f(vvi& el, vi& disc, vi& low, vi& dads, si& aps,
    int n, int time) {
  disc[n] = low[n] = (++time);

  //pd(); cerr << "n = " << n << " disc = " << time << endl;

  int kids = 0;
  for (int k : el[n]) {
    //pd(); cerr << "  k = " << k;
    if (disc[k] != -1) { // vis check
      if (k != dads[n]) low[n] = min(low[n], disc[k]);
      //cerr << " vis. low[n] = " << low[n] << endl;
    } else {
      //cerr << " not vis" << endl;
      ++kids;
      dads[k] = n;
      //++d;
      time = artic_f(el, disc, low, dads, aps, k, time);
      //--d;
      low[n] = min(low[n], low[k]);
      //pd(); cerr << "  low[n] = " << low[n] << endl;
      
      if (dads[n] != -1 && low[k] >= disc[n]) {
        //pd(); cerr << "  ap!" << endl;
        aps.insert(n);
      }
    }
  }
  
  if (dads[n] == -1 && kids > 1) aps.insert(n);

  return time;
} 

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
        //cerr << n << " <-> " << k << endl;
      }
    }

    vi disc(N, -1);
    vi low(N, -1);
    vi dads(N, -1);
    si aps;
    artic_f(el, disc, low, dads, aps, 0, 0);

    cout << aps.size() << endl;

  }
}

int main() {
  solve_network();
}
