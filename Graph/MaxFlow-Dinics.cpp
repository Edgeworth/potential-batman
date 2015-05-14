#include "../Defines.h"

/* START SOLUTION */

// populates the array d to hold the "level" (i.e. number of
// edges away from s) of each node
// the graph wherein every edge connect a node to a node of
// level one higher is called the "level graph"
// c      : capacity graph (adj matrix)
// f      : flow graph (adj matrix)
// s      : source node
// t      : dest node
// d      : level list (must be pre-allocated to right size)
// returns: true if t is connected to s in the level graph
bool level(vvl& c, vvl& f, int s, int t, vi& d) {
  fill(d.begin(), d.end(), -1);
  d[s] = 0;
  
  queue<int> q;
  q.push(s);
  
  while(!q.empty()) {
    int u = q.front();
    q.pop();
    
    for(int v = 0; v < c[u].size(); ++v) {
      if(c[u][v] - f[u][v] <= 0 || d[v] >= 0) continue;

      d[v] = d[u] + 1;
      q.push(v);
    }
  }
  
  return d[t] >= 0;
}

// pushes flow through all shortest paths at once
// c      : the capacity graph
// f      : the current flow
// u      : the current node being examined
// b      : the bottleneck on the path traversed so far
//          (initially call with b = INF)
// returns: the improvement in flow
ll aug_path(vvl& c, vvl& f, vi& d, int t, int u, ll b) {
  if(u == t) return b;
  
  ll tf = 0;
  for(int v = 0; tf < b && v < c[u].size(); ++v) {
    if(d[v] != d[u] + 1 || c[u][v] - f[u][v] <= 0) continue;
    
    ll af = aug_path(c, f, d, t, v,
        min(b - tf, c[u][v] - f[u][v]));

    if(af > 0) { // augment path
      f[u][v] += af;
      f[v][u] -= af;
      tf += af;
    }
    else d[v] = -1; // burn edge 
  }
  return tf;
}

const static ll INF = LLONG_MAX;

// calculates the maximum flow through the given graph
// c      : the capacity graph
// f      : the flow graph (should be initially passed in at
//          correct size and filled with 0s) to be populated 
// s      : the source node
// t      : the sink node
// returns: the maximum flow through the given graph
ll max_flow(vvl& c, vvl& f, int s, int t) {
  vi d(c.size());
  ll tf = 0;
  
  while(level(c, f, s, t, d))
    tf += aug_path(c, f, d, t, s, INF);
  
  return tf;
}

/* END SOLUTION */

// solves http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=761
int solve_bandwidth() {
  int kase = 0;
  while(true) {
    int n;
    cin >> n;

    if(n == 0)
      return 0;

    vvl cap(n, vl(n, 0));
    vvl flow(n, vl(n, 0));

    int s, t, c;
    cin >> s >> t >> c;

    for(int i = 0; i < c; ++i) {
      int x, y, z;
      cin >> x >> y >> z;

      cap[x-1][y-1] += z;
      cap[y-1][x-1] += z;
    }

    cout << "Network " << ++kase << endl;
    cout << "The bandwidth is " << max_flow(cap, flow, s-1, t-1) << "." << endl << endl;
  }
}

// solves http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&category=&problem=1021
int solve_gopher() {
  while(!(cin>>ws).eof()) {
    int n, m, s, v;
    cin >> n >> m >> s >> v;

    const int N = n + m + 2;
    vvl cap(N, vl(N, 0));
    vvl flow(N, vl(N, 0));

    vector<vec2> moles(n);
    for(int i = 0; i < n; ++i) {
      cin >> get<0>(moles[i])  >> get<1>(moles[i]);
      cap[0][2+i] = 1;
    }

    for(int j = 0; j < m; ++j) {
      double x, y;
      cin >> x >> y;
      cap[2+n+j][1] = 1;

      for(int i = 0; i < n; i++) {
        double gx, gy;
        tie(gx, gy) = moles[i];
        double dx = gx - x, dy = gy - y;
        if(v * s > sqrt(dx * dx + dy * dy))
          cap[2+i][2+n+j] = 1;
      }
    }

    cout << n - max_flow(cap, flow, 0, 1) << endl;
  }

  return 0;
}

int main() {
  return solve_gopher();
}
