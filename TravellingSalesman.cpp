#include "Defines.h"

/* START SOLUTION */

// executes the O(n^2 2^n) TSP DP
// the subproblem is: "what is the minimum cost
// of starting at node 0, ending at node t and
// visiting all nodes in vis?"
// g      : the (not necessarily complete, not
//          necessarily symmetric) TSP instance
// dp     : a 2^n x n array for holding results
// vis    : a bitmask representing visited nodes
// t      : the last visited node
// n      : the number of nodes in the instance
// returns: the cost of the minimum cycle through
//          all nodes
ll tsp(const vvll& g, vvll& dp, ll vis,
       int t, int n) {
    // -1 means not yet calculated
    if(dp[vis][t] == -1) {
        const ll pvis = vis & (~(1<<t));

        // base case
        if(pvis == 0) {
            // -2 means invalid
            if(g[0][t] < 0)
                dp[vis][t] = -2;
            else
                dp[vis][t] = g[0][t];
        }
        else {
            ll mi = -2; // could be impossible
            for(int s = 0; s < n; ++s) {
                // if we have visited s and
                // there is a path from s to t
                if((pvis & (1<<s)) != 0 && 
                    g[s][t] >= 0) {
                    ll rec = tsp(g, dp, pvis, s, n);
                    ll dist = rec + g[s][t];

                    if(rec >= 0 &&
                       (mi < 0 || mi > dist))
                        mi = dist;
                }
            }
            dp[vis][t] = mi;
        }
    }

    return dp[vis][t];
}

ll tsp(const vvll& g) {
    vvll dp(1<<g.size(), vll(g.size(), -1));
    return tsp(g, dp, (1<<g.size())-1, 0, g.size());
}

/* END SOLUTION */

// solves http://uva.onlinejudge.org/index.php?option=onlinejudge&Itemid=99999999&page=submit_problem&problemid=1437&category=
int solve_beepers() {
    int T;
    cin >> T;

    for(int t = 0; t < T; ++t) {
        int N;
        cin >> N >> N; // throw away world size

        vector<tuple<int, int>> ps(1);
        cin >> get<0>(ps[0]) >> get<1>(ps[0]);

        cin >> N;
        ps.resize(1 + N);
        for(int n = 1; n <= N; ++n)
            cin >> get<0>(ps[n]) >> get<1>(ps[n]);
        //cout << "size = " << ps.size();
        
        vvll g(ps.size(), vll(ps.size()));
        for(int v1 = 0; v1 < ps.size(); ++v1)
            for(int v2 = 0; v2 < ps.size(); ++v2)
                g[v1][v2] = abs(get<0>(ps[v1]) - get<0>(ps[v2])) +
                            abs(get<1>(ps[v1]) - get<1>(ps[v2]));
        
        cout << "The shortest path has length " << tsp(g) << endl;
    }
    return 0;
}

// solves example from http://en.wikipedia.org/wiki/Held%E2%80%93Karp_algorithm
// optimal result = 21
int test() {
    vvll g = {{ 0, 2,  9, 10},
              { 1, 0,  6,  4},
              {15, 7,  0,  8},
              { 6, 3, 12,  0}};
    cout << tsp(g) << endl;
    return 0;
}

int main() {
    return solve_beepers();
}
