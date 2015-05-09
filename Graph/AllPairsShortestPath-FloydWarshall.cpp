#include "../Defines.h"

/* START SOLUTION */

// dists : adj matrix with LLONG_MAX/2 representing no edge
// next  : next[i][j] is the next node on the path i to j. 
//         Start next[i][j] = i for each edge i,j
void floyd_warshall(vvl& dists, vvi& next) {
  int N = dists.size();
  for (int k = 0; k < N; k++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (dists[i][k] + dists[k][j] < dists[i][j]) {
          dists[i][j] = dists[i][k] + dists[k][j];
          next[i][j] = next[i][k];
        }
      }
    }
  }
}

bool neg_cycle_after_fw(vvl& dists) {
  for (int i = 0; i < dists.size(); i++)
    if (dists[i][i] < 0) return true;
  return false;
}

/* END SOLUTION */

void simple_test() {
  ll I = LLONG_MAX/2;
  vvl dists = {
    {0, 9, 5, I, 4},
    {7, 0, I, I, 2},
    {I, 9, 0, 8, I},
    {3, I, I, 0, 4},
    {I, 9, 5, I, 0},
  };
  int N = dists.size();
  vvi next(N, vi(N, -1));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      if (i != j && dists[i][j] < I) next[i][j] = j;

  for (auto& a : dists) {
    for (ll d : a) cout << d << " ";
    cout << endl;
  }

  floyd_warshall(dists, next);

  cout << "after: " << endl;
  for (auto& a : dists) {
    for (ll d : a) cout << d << " ";
    cout << endl;
  }
  cout << endl << "next: " << endl;
  for (auto& a : next) {
    for (int d : a) cout << d << " ";
    cout << endl;
  }
}

//http://uva.onlinejudge.org/external/1/186.html
typedef map<pair<int, int>, pair<ll, string>> em_t;

int get_id(map<string, int>& cti, vs& itc, string city) {
  if (cti.find(city) == cti.end()) {
    cti[city] = itc.size();
    itc.push_back(city);
  }
  return cti[city];
}

void solve_trip_routing() {
  map<string, int> cti; // city to id
  vs itc; // id to city
  em_t edge_map;
  while (true) {
    string line;
    getline(cin, line);
    if (line.size() == 0) break;

    int pos = line.find(',');
    int a = get_id(cti, itc, line.substr(0, pos));
    int pos2 = line.find(',', ++pos);
    int b = get_id(cti, itc, line.substr(pos, pos2 - pos));
    int pos3 = line.find(',', ++pos2);
    ll w = atol(line.substr(pos3+1).c_str());
    if (edge_map.find({a,b}) != edge_map.end() &&
        edge_map[{a,b}].first < w) continue;
    edge_map[{a,b}] = {w, line.substr(pos2, pos3 - pos2)};
  }
  int N = cti.size();

  vvl adj(N, vl(N, LLONG_MAX/2));
  vvi next(N, vi(N, -1));
  for (int i = 0; i < N; i++) adj[i][i] = 0;
  for (auto& kv : edge_map) {
    int i = kv.first.first;
    int j = kv.first.second;
    adj[i][j] = kv.second.first;
    adj[j][i] = kv.second.first;
    next[i][j] = j;
    next[j][i] = i;
  }

  floyd_warshall(adj, next);

  while (!cin.eof()) {
    string line;
    getline(cin, line);
    if (line.size() == 0) break;

    int pos = line.find(',');
    int f = cti[line.substr(0, pos)];
    int t = cti[line.substr(pos+1)];
    ll dist = adj[f][t];

    cout << "\n\n";
    cout << "From                 To                   Route      Miles\n";
    cout << "-------------------- -------------------- ---------- -----\n";
    while (f != t) {
      int a = f;
      int n = next[f][t];

      cout << left;
      cout << setw(21) << itc[a];
      cout << setw(21) << itc[n];
      if (edge_map.find({a,n}) == edge_map.end()) swap(a,n);
      cout << setw(11) << edge_map[{a,n}].second;
      cout << setw(5) << right << edge_map[{a,n}].first << left;
      cout << endl;

      f = next[f][t];
    }
    cout << "                                                     -----\n";
    cout << "                                          Total      " << setw(5) << right << dist << left << endl;
  }
}

int main() {
  //simple_test();

  solve_trip_routing();
}
