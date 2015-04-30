#include "Defines.h"

/* START SOLUTION */

// O(|E| * lg(|E|))
// el    : an edge list of the form {to, weight}
// edges : empty vec to store the answer
// returns the wight of the minimum spanning tree or -1 if 
//         not connected
int prims(vvpii& el, vtiii& edges) {
  int N = el.size();
  vb vis(N);
  set<tiii> pq; //weight, from, to
  int tot = 0;

  pq.insert(make_tuple(0, 0, 0)); // dummy edge
  while (!pq.empty()) {
    int w, f, t;
    tie(w, f, t) = *pq.begin();
    pq.erase(pq.begin());
    if (vis[t]) continue;

    vis[t] = true;
    tot += w;
    if (f != t) // ignore dummy edge
      edges.push_back(make_tuple(w, f, t));
    for (auto& e : el[t])
      pq.insert(make_tuple(e.second, t, e.first));
  }

  return (edges.size() == N-1) ? tot : -1;
}

/* END SOLUTION */

// http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=3649
int main() {
  int cases;
  cin >> cases;  
  for (int caze = 1; caze <= cases; caze++) {
    int N;
    cin >> N;
    vvpii el(N, vpii());
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        int w;
        cin >> w;
        if (w > 0) el[i].push_back({j, w});
        string s;
        if (j != N-1) cin >> s;
      }
    }

    cout << "Case " << caze << ": " << endl;
    vtiii ans;
    prims(el, ans);
    for (auto& e : ans)
      if (get<1>(e) > get<2>(e)) swap(get<1>(e), get<2>(e));
    sort(ans.begin(), ans.end());
    for (auto& e : ans) {
      int w, f, t;
      tie(w, f, t) = e;
      cout << (char)('A' + f) << "-" << (char)('A' + t) << 
        " " << w << endl;
    }
  }
}
