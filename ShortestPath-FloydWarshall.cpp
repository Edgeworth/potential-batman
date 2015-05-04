#include "Defines.h"

typedef vector<ll> vl;
typedef vector<vl> vvl;

/* START SOLUTION */

// dists : adj matrix with LLONG_MAX/2 representing no edge
// dads  : dads[i][j] is the penultimate node on the path i
//         to j. Start dads[i][j] = i for each edge i,j
void floyd_warshall(vvl& dists, vvi& dads) {
  int N = dists.size();
  for (int k = 0; k < N; k++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (k == i || k == j) continue; // TODO
        if (dists[i][k] + dists[k][j] < dists[i][j]) {
          dists[i][j] = dists[i][k] + dists[k][j];
          dads[i][j] = dads[k][j];
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
  vvi dads(N, vi(N, -1));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      if (i != j && dists[i][j] < I) dads[i][j] = i;

  for (auto& a : dists) {
    for (ll d : a) cout << d << " ";
    cout << endl;
  }

  floyd_warshall(dists, dads);

  cout << "after: " << endl;
  for (auto& a : dists) {
    for (ll d : a) cout << d << " ";
    cout << endl;
  }
  cout << endl << "dads: " << endl;
  for (auto& a : dads) {
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
  vvi dads(N, vi(N, -1));
  for (int i = 0; i < N; i++) adj[i][i] = 0;
  for (auto& kv : edge_map) {
    int i = kv.first.first;
    int j = kv.first.second;
    adj[i][j] = kv.second.first;
    adj[j][i] = kv.second.first;
    dads[i][j] = i;
    dads[j][i] = j;
  }

  floyd_warshall(adj, dads);

  while (!cin.eof()) {
    string line;
    getline(cin, line);
    if (line.size() == 0) break;

    int pos = line.find(',');
    int f = cti[line.substr(0, pos)];
    int t = cti[line.substr(pos+1)];
    ll dist = adj[f][t];

    vpii r;
    while (t != f) {
      r.push_back({dads[f][t], t});
      t = dads[f][t];
    }

    cout << "\n\n";
    cout << "From                 To                   Route      Miles\n";
    cout << "-------------------- -------------------- ---------- -----\n";
    for (int i = r.size()-1; i >= 0; --i) {
      int a = r[i].first;
      int b = r[i].second;
      cout << left;
      cout << setw(21) << itc[a];
      cout << setw(21) << itc[b];
      if (edge_map.find({a,b}) == edge_map.end()) swap(a,b);
      cout << setw(11) << edge_map[{a,b}].second;
      cout << setw(5) << right << edge_map[{a,b}].first << left;
      cout << endl;
    }
    cout << "                                                     -----\n";
    cout << "                                          Total      " << setw(5) << right << dist << left << endl;
  }
}

int main() {
  //simple_test();

  solve_trip_routing();
}
