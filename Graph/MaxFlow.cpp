#include "Defines.h"

/* START SOLUTION */

// cap    : matrix of edge capacities
// flow   : matrix to be filled with edge flows (must start
//          zeroed)
// s      : source
// t      : sink
// returns: max flow
ll max_flow(vvl& cap, vvl& flow, int s, int t) {
  ll tot_flow = 0;

  // keep pushing augmenting flows
  while(true) {
    vi p(cap.size(), -1);
    p[s] = s;

    // BFS starts here
    queue<int> q;
    q.push(s);
    while(!q.empty()) {
      int cur = q.front();
      q.pop();

      for(int next = 0; next < cap.size(); ++next) {
        if(cap[cur][next] - flow[cur][next] <= 0 ||
           p[next] != -1)
          continue;

        q.push(next);
        p[next] = cur;
      }
    }
    // BFS ends here
    
    if(p[t] == -1) {
      return tot_flow;
    }
    else {
      // find bottleneck
      ll bottleneck = LLONG_MAX;
      int cur = t;
      while(p[cur] != cur) {
        bottleneck = min(bottleneck, cap[p[cur]][cur] -
                         flow[p[cur]][cur]);
        cur = p[cur];
      }

      // update flow
      cur = t;
      while(p[cur] != cur) {
        flow[p[cur]][cur] += bottleneck;
        flow[cur][p[cur]] -= bottleneck;
        cur = p[cur];
      }

      tot_flow += bottleneck;
    }
  }
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
  return solve_bandwidth();
}
