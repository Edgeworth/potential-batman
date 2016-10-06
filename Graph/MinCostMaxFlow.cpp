#include <bits/stdc++.h>
using namespace std;

// From Max Ward.
// Both SPFA and Dijkstra tested on:
// http://www.spoj.com/problems/GREED/
// https://code.google.com/codejam/contest/11254486/dashboard#s=p2
// http://ukiepc.info/2015/ Aqueduct Construction
/* START SOLUTION */
typedef int cost_t; // If this is double, use an EPS for edge relaxations.
typedef int cap_t;
const cap_t CAPINF = numeric_limits<cap_t>::max()/2;
const cost_t COSTINF = numeric_limits<cost_t>::max()/2;
struct MCMF { // O(max_flow * augment)
  int N;
  vector<vector<int>> als;
  vector<vector<cost_t>> costs, revcosts;
  vector<vector<cap_t>> caps, revcaps;
  vector<cost_t> D; vector<int> parent; // Both used by augment
  void augment_spfa(int s) {  // Fills parent and D.
    parent.assign(N, -1);   // Faster than dijk for random graphs.
    D.assign(N, COSTINF);
    vector<bool> inQ(N);
    D[s] = 0; inQ[s] = true;
    queue<int> q; q.push(s);
    while (!q.empty()) { // Loops forever on negative cycle.
      int at = q.front(); q.pop();
      inQ[at] = false;
      for (int c : als[at]) if (caps[at][c] > 0 || revcaps[at][c] > 0) {
        cost_t w = COSTINF;
        if (caps[at][c] > 0) w = min(w, costs[at][c]);
        if (revcaps[at][c] > 0) w = min(w, revcosts[at][c]);
        if (D[at] + w < D[c]) {
          D[c] = D[at] + w;
          parent[c] = at;
          if (!inQ[c]) { q.push(c); inQ[c] = true; }
        }}}
  }
  // Dijkstras can be worth it if SPFA is getting rekt by non-random input
  vector<cost_t> pi;
  void augment_dijks(int s) {
    if (pi.size() == 0) { // Get rid of negative edge costs
      pi.assign(N+1, 0); // For dijkstra potentials. N+1 for special node
      vector<int> tmp(N); // Can remove the rest of this for no-neg weights input
      for (int i = 0; i < N; ++i) tmp[i] = i;
      als.push_back(tmp); costs.push_back(vector<cost_t>(N)); // Special node
      for (int i = 0; i < N; ++i) // Bellman-ford V-1 iterations
        for (int u = 0; u <= N; ++u) for (int c : als[u]) if (u >= N || caps[u][c]) {
          pi[c] = min(pi[c], costs[u][c] + pi[u]);
        }
      als.pop_back(); costs.pop_back();
    }
    parent.assign(N, -1);
    D.assign(N, COSTINF); D[s] = 0;
    set<tuple<cost_t, int, int>> q;
    q.insert(make_tuple(0,s,s));
    while (!q.empty()) {
      auto top = *q.begin(); q.erase(q.begin());
      cost_t w = get<0>(top); int u = get<1>(top), p = get<2>(top);
      if (parent[u] != -1 || w > D[u]) continue;
      parent[u] = p;
      for (int v : als[u]) if (caps[u][v] > 0 || revcaps[u][v] > 0) {
        cost_t ew = COSTINF;
        if (caps[u][v] > 0) ew = min(ew, costs[u][v]);
        if (revcaps[u][v] > 0) ew = min(ew, revcosts[u][v]);
        ew += pi[u] - pi[v];
        if (ew+w < D[v]) {
          D[v] = ew+w;
          q.insert(make_tuple(ew+w, v, u));
        }}}
    for (int i = 0; i < N; ++i) if (parent[i] != -1)
      pi[i] += D[i]; // Maintain no neg cost edges
  }
  pair<cost_t, cap_t> run(const vector<vector<cap_t>> &cap,
      const vector<vector<cost_t>> &cost, int s, int t) {
    N = cap.size();
    als.assign(N, vector<int>());
    costs = cost; revcosts = costs;
    caps = cap; revcaps.assign(N, vector<cap_t>(N));
    for (int u = 0; u < N; ++u) { // Construct adj lists for flow graph.
      for (int v = 0; v < N; ++v)  {
        revcosts[u][v] = -costs[v][u];
        if (cap[u][v] > 0) {
          als[u].push_back(v); als[v].push_back(u);
        }}}
    cap_t flow = 0; cost_t fcost = 0;
    augment_spfa(s);
    while (parent[t] != -1) { // Keep finding augmenting paths.
      cap_t bottle = CAPINF;
      for (int at = t; at != s; at = parent[at]) { // Find bottleneck.
        int p = parent[at]; // If you want min cost flow (not max flow)
        bottle = min(bottle, revcaps[p][at] == 0 // keep sum of edge costs in this loop
          || costs[p][at] < revcosts[p][at] ? caps[p][at] : revcaps[p][at]);
      } // then if the sum is > 0, break
      flow += bottle;
      for (int at = t; at != s; at = parent[at]) { // Push the flow.
        int p = parent[at];
        if (revcaps[p][at] == 0 || costs[p][at] < revcosts[p][at]) {
          fcost += costs[p][at]*bottle;
          caps[p][at] -= bottle; revcaps[at][p] += bottle;
        } else {
          fcost += revcosts[p][at]*bottle;
          revcaps[p][at] -= bottle; caps[at][p] += bottle;
        }
      }
      augment_spfa(s);
    }
    return {fcost, flow};
  }
};
/* END SOLUTION */

// the maximum number of vertices + 1
#define NN 1024

// adjacency matrix (fill this up)
int cap[NN][NN];

// cost per unit of flow matrix (fill this up)
int cost[NN][NN];

// flow network and adjacency list
int fnet[NN][NN], adj[NN][NN], deg[NN];

// Dijkstra's predecessor, depth and priority queue
int par[NN], d[NN], q[NN], inq[NN], qs;

// Labelling function
int pi[NN];

#define CLR(a, x) memset( a, x, sizeof( a ) )
#define Inf (INT_MAX/2)
#define BUBL { \
  t = q[i]; q[i] = q[j]; q[j] = t; \
  t = inq[q[i]]; inq[q[i]] = inq[q[j]]; inq[q[j]] = t; }

// Dijkstra's using non-negative edge weights (cost + potential)
#define Pot(u,v) (d[u] + pi[u] - pi[v])
bool dijkstra( int n, int s, int t )
{
  CLR( d, 0x3F );
  CLR( par, -1 );
  CLR( inq, -1 );
  //for( int i = 0; i < n; i++ ) d[i] = Inf, par[i] = -1;
  d[s] = qs = 0;
  inq[q[qs++] = s] = 0;
  par[s] = n;

  while( qs )
  {
    // get the minimum from q and bubble down
    int u = q[0]; inq[u] = -1;
    q[0] = q[--qs];
    if( qs ) inq[q[0]] = 0;
    for( int i = 0, j = 2*i + 1, t; j < qs; i = j, j = 2*i + 1 )
    {
      if( j + 1 < qs && d[q[j + 1]] < d[q[j]] ) j++;
      if( d[q[j]] >= d[q[i]] ) break;
      BUBL;
    }

    // relax edge (u,i) or (i,u) for all i;
    for( int k = 0, v = adj[u][k]; k < deg[u]; v = adj[u][++k] )
    {
      // try undoing edge v->u
      if( fnet[v][u] && d[v] > Pot(u,v) - cost[v][u] )
        d[v] = Pot(u,v) - cost[v][par[v] = u];

      // try using edge u->v
      if( fnet[u][v] < cap[u][v] && d[v] > Pot(u,v) + cost[u][v] )
        d[v] = Pot(u,v) + cost[par[v] = u][v];

      if( par[v] == u )
      {
        // bubble up or decrease key
        if( inq[v] < 0 ) { inq[q[qs] = v] = qs; qs++; }
        for( int i = inq[v], j = ( i - 1 )/2, t;
            d[q[i]] < d[q[j]]; i = j, j = ( i - 1 )/2 )
          BUBL;
      }
    }
  }

  for( int i = 0; i < n; i++ ) if( pi[i] < Inf ) pi[i] += d[i];

  return par[t] >= 0;
}
#undef Pot

int mcmf4( int n, int s, int t, int &fcost )
{
  // build the adjacency list
  CLR( deg, 0 );
  for( int i = 0; i < n; i++ )
    for( int j = 0; j < n; j++ )
      if( cap[i][j] || cap[j][i] ) adj[i][deg[i]++] = j;

  CLR( fnet, 0 );
  CLR( pi, 0 );
  int flow = fcost = 0;

  // repeatedly, find a cheapest path from s to t
  while( dijkstra( n, s, t ) )
  {
    // get the bottleneck capacity
    int bot = INT_MAX;
    for( int v = t, u = par[v]; v != s; u = par[v = u] )
      bot = min(bot, fnet[v][u] ? fnet[v][u] : ( cap[u][v] - fnet[u][v] ));

    // update the flow network
    for( int v = t, u = par[v]; v != s; u = par[v = u] )
      if( fnet[v][u] ) { fnet[v][u] -= bot; fcost -= bot * cost[v][u]; }
      else { fnet[u][v] += bot; fcost += bot * cost[u][v]; }

    flow += bot;
  }

  return flow;
}


void test() {
  int MAXN = 20, tests = 50000, max_cap = 10, max_cost = 10;
  double density = 0.2;
  default_random_engine re;
  for (int t = 1; t <= tests; ++t) {
    int N = re()%MAXN + 2;
    vector<vector<cap_t>> capp(N, vector<cap_t>(N));
    vector<vector<cost_t>> costp(N, vector<cost_t>(N));
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j) {
        cap[i][j] = 0; cost[i][j] = 0;
        if (((re()%100)+1)*0.01 > density) continue;
        capp[i][j] = cap[i][j] = re()%max_cap;
        costp[i][j] = cost[i][j] = re()%max_cost;
      }
    int c,  f;
    c = mcmf4(N, N-2, N-1, f);
    MCMF mcmf;
    auto res = mcmf.run(capp, costp, N-2, N-1);
    clog << res.first << " " << res.second << " " << f << " " << c << " " << endl;
    if (res.first != f || res.second != c) {
        for (int i = 0; i < N; ++i) {
          for (int j = 0; j < N; ++j) {
            cout << cap[i][j] << "\t";
          }
          cout << endl;
        }
        for (int i = 0; i < N; ++i) {
          for (int j = 0; j < N; ++j) {
            cout << cost[i][j] << "\t";
          }
          cout << endl;
        }
      cout << "Test failed" << endl;
      return;
    }
  }
  cout << "All tests passed" << endl;
}

int main() {
  test();
}
